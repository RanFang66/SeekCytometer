#include "UdpCommFrame.h"


UdpCommFrame::UdpCommFrame() {}


quint16 UdpCommFrame::checkSum(const QByteArray &frameWithoutCheckSum)
{
    quint16 sum = 0;
    for (int i = 0; i < frameWithoutCheckSum.size(); ++i) {
        sum += static_cast<quint8>(frameWithoutCheckSum.at(i));
    }
    return sum;
}


QByteArray UdpCommFrame::packFrame(quint16 sequence, CommCmdType cmdType, const QByteArray &data)
{
    // Frame layout:
    // [0xA5 | 0x5A(1B)] + [sequence (2B)] + [commandType (2B)]
    // + [dataLen (2B)] + [data (N B)] + [checkSum (2B)]
    QByteArray frame;
    frame.append(FRAME_HEADER_TO_SOC);
    frame.append(static_cast<char>((sequence >> 8) & 0xFF));
    frame.append(static_cast<char>(sequence & 0xFF));
    frame.append(static_cast<char>((static_cast<quint8>(cmdType) >> 8) & 0xFF));
    frame.append(static_cast<char>(static_cast<quint8>(cmdType) & 0xFF));
    frame.append(static_cast<char>((data.size() >> 8) & 0xFF));
    frame.append(static_cast<char>(data.size() & 0xFF));
    frame.append(data);
    quint16 checksum = checkSum(frame);
    frame.append(static_cast<char>((checksum >> 8) & 0xFF));
    frame.append(static_cast<char>(checksum & 0xFF));
    return frame;
}

bool UdpCommFrame::verifyFrame(const QByteArray &frame)
{
    if (frame.size() < 9) {
        return false;
    }
    if (frame.at(0) != FRAME_HEADER_FROM_SOC) {
        return false;
    }
    quint16 dataLen = (static_cast<quint8>(frame.at(5)) << 8) | static_cast<quint8>(frame.at(6));
    if (frame.size() != dataLen + 9) {
        return false;
    }
    quint16 checksum = (static_cast<quint8>(frame.at(dataLen + 7)) << 8) | static_cast<quint8>(frame.at(dataLen + 8));
    QByteArray frameWithoutChecksum = frame.left(dataLen + 7);
    return checkSum(frameWithoutChecksum) == checksum;
}

bool UdpCommFrame::tryParseFrame(QByteArray &buffer, QByteArray &frame)
{
    if (buffer.size() < 9) {
        return false;
    }
    if (buffer.at(0) != FRAME_HEADER_FROM_SOC) {
        buffer.remove(0, 1);
        return false;
    }
    quint16 dataLen = (static_cast<quint8>(buffer.at(5)) << 8) | static_cast<quint8>(buffer.at(6));
    int expectedSize = 1 + 2 + 2 + 2 + dataLen + 2;
    // If we haven't received enough bytes for a complete frame, wait for more data
    if (buffer.size() < expectedSize) {
        return false;
    }
    // Extract one frame
    QByteArray oneFrame = buffer.left(expectedSize);
    // Verify the frame
    if (!verifyFrame(oneFrame)) {
        // If verification fails, remove one byte and retry
        buffer.remove(0, 1);
        return false;
    }
    // If valid, set 'frame' and remove those bytes from 'buffer'
    frame = oneFrame;
    buffer.remove(0, expectedSize);
    return true;
}

quint16 UdpCommFrame::getSequence(const QByteArray &frame)
{
    return (static_cast<quint8>(frame.at(1)) << 8) | static_cast<quint8>(frame.at(2));
}

CommCmdType UdpCommFrame::getCommandType(const QByteArray &frame)
{
    return static_cast<CommCmdType>((static_cast<quint8>(frame.at(3)) << 8) | static_cast<quint8>(frame.at(4)));
}

QByteArray UdpCommFrame::getDataField(const QByteArray &frame)
{
    quint16 dataLen = (static_cast<quint8>(frame.at(5)) << 8) | static_cast<quint8>(frame.at(6));
    return frame.mid(7, dataLen);
}





