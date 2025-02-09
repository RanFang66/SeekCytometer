#ifndef NETWORKFRAME_H
#define NETWORKFRAME_H

#include <QObject>

class NetworkFrame : public QObject
{
    Q_OBJECT
public:
    enum CMD_TYPE {
        DETECT_SERVER       = 0x5A01,
        CONFIG_ANALYZER     = 0x5A02,
        GET_CH_WAVEFORM     = 0x5A03,
        GET_CH_PULSE_RES    = 0x5A04,
        SET_CH_THRESH       = 0x5A10,
        SET_DRIVE_PARAS     = 0x5A11,
    };
    enum CHANNEL_INDEX {
        CH_0 = 0,
        CH_1,
        CH_2,
        CH_3,
        CH_4,
        CH_5,
        CH_6,
        CH_7,
    };

    explicit NetworkFrame(QObject *parent = nullptr);

    NetworkFrame(CMD_TYPE cmdType, CHANNEL_INDEX channel, const QByteArray &data, QObject *parent = nullptr);


    const QByteArray& getSendBuff() const;

signals:

private:
    quint8          m_frameHeader = 0;
    CMD_TYPE        m_cmdType;
    CHANNEL_INDEX   m_channel;
    QByteArray      m_data;
    QByteArray      m_sendBuff;
    quint8 checkSum(const QByteArray &data);
};

#endif // NETWORKFRAME_H
