#ifndef UDPCOMMFRAME_H
#define UDPCOMMFRAME_H

#include <QObject>


/**
 * @brief A helper class for packing and parsing custom UDP frames.
 *
 * Frame format:
 *   [0xA5|0x5A (1 byte)] + [sequence (2 bytes)] + [commandType (2 bytes)]
 *   + [dataLen (2 bytes)] + [data (N bytes)] + [checkSum (2 bytes)]
 */
enum class CommCmdType {
    CMD_HAND_SHAKE = 0x01,
    CMD_DETECTOR_SETTINGS = 0x02,
    CMD_DISABLE_DETECTOR = 0x03,

    CMD_ACQUIRE_SETTINGS = 0x04,
    CMD_ACQUIRE_START = 0x05,
    CMD_ACQUIRE_STOP = 0x06,

    CMD_DRIVE_SETTINGS = 0x07,
    CMD_SORTING_START = 0x08,
    CMD_SORTING_STOP = 0x09,

    CMD_GATE_SETTINGS = 0x0A,

    CMD_PULSE_DATA = 0x10,
    CMD_WAVEFORM_DATA = 0x11,
};

class UdpCommFrame
{
public:
    UdpCommFrame();
    static constexpr quint8 FRAME_HEADER_TO_SOC = 0xA5;
    static constexpr quint8 FRAME_HEADER_FROM_SOC = 0x5A;


    /**
     * @brief Calculates the 16-bit checksum for the frame.
     * @param frameWithoutChecksum The frame content excluding the final 2-byte checksum.
     * @return 16-bit checksum value.
     */
    static quint16 checkSum(const QByteArray &data);

    /**
     * @brief Packs raw data into a complete frame according to the protocol.
     * @param sequence The 16-bit frame sequence number.
     * @param commandType The 16-bit command type.
     * @param data The payload data.
     * @return A complete frame in QByteArray.
     */
    static QByteArray packFrame(quint16 sequence, CommCmdType cmdType, const QByteArray &data);

    /**
     * @brief Verifies if a frame is valid (checks frame flag, length, and checksum).
     * @param frame The frame to verify.
     * @return true if valid; false otherwise.
     */
    static bool verifyFrame(const QByteArray &frame);

    /**
     * @brief Attempts to parse one complete frame from the buffer.
     * @param buffer The raw data buffer which may contain multiple frames or partial frames.
     * @param frame If parsing succeeds, one complete frame is extracted.
     * @return true if a complete valid frame is found, false otherwise.
     *
     * On success, the used bytes are removed from 'buffer'.
     */
    static bool tryParseFrame(QByteArray &buffer, QByteArray &frame);

    /**
     * @brief Extracts the 16-bit sequence number from a valid frame.
     */
    static quint16 getSequence(const QByteArray &frame);

    /**
     * @brief Extracts the command type from a valid frame.
     */
    static CommCmdType getCommandType(const QByteArray &frame);

    /**
     * @brief Extracts the payload data field from a valid frame.
     */
    static QByteArray getDataField(const QByteArray &frame);

};

#endif // UDPCOMMFRAME_H
