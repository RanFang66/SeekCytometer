#include "NetworkFrame.h"

NetworkFrame::NetworkFrame(QObject *parent)
    : QObject{parent}
{}

NetworkFrame::NetworkFrame(CMD_TYPE cmdType, CHANNEL_INDEX channel, const QByteArray &data, QObject *parent)
    :    QObject{parent},
    m_cmdType(cmdType),
    m_channel(channel),
    m_data(data)
{
    m_sendBuff.append(m_frameHeader);
    m_sendBuff.append((char)(m_cmdType >>8));
    m_sendBuff.append((char)(m_cmdType));
    m_sendBuff.append((char)(m_channel>>8));
    m_sendBuff.append((char)(m_channel));


    int dataLen = m_data.length();
    m_sendBuff.append((char)(dataLen>>8));
    m_sendBuff.append((char)(dataLen));
    m_sendBuff.append(m_data);
    quint8 chksum = checkSum(m_sendBuff);
    m_sendBuff.append(chksum);
    m_frameHeader += 0x02;
}


quint8 NetworkFrame::checkSum(const QByteArray &data)
{
    quint8 sum = 0;
    for (qint64 i = 0; i < data.size(); i++) {
        sum += data.at(i);
    }
    return sum;
}

const QByteArray &NetworkFrame::getSendBuff() const
{
    return m_sendBuff;
}


