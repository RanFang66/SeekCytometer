#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QNetworkInterface>
#include <QtNetwork>
#include "NetworkFrame.h"

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    static NetworkClient *getInstance(QObject *parent = nullptr) {
        static NetworkClient _instance(parent);
        return &_instance;
    }

    const QString getLocalIpv4() const;

    QString getUdpClientState();



signals:
    void receivedServerInfoFrame(QString ip, quint16 port);

    void receivedWaveFormFrame(int channel, const QList<qreal> &);

    void receivedPulseResFrame(const QByteArray &);

    void receivedPulseThreshFrame(int channel, int validTh, int sortTh);

    void receivedDriveParametersFrame(int driveDelay, int driveWidth, int coolTime, int sortEnCh);

public slots:
    void detectServer();

    void getWaveform(int sampleCh, int sampleLen = 32 * 1024);

    void setChannelSetting(NetworkFrame::CHANNEL_INDEX channel);

    void setAnalyzerState(bool enable);

    void setDriveSetting();


private slots:
    // void onUdpSocketStateChanged(QUdpSocket::SocketState);
    void onUdpSocketReadyRead();


private:
    explicit NetworkClient(QObject *parent = nullptr);
    const NetworkClient &operator=(const NetworkClient &client) = delete;
    NetworkClient(const NetworkClient &client) = delete;

    void initNetworkClient();

    quint8 checkSum(const QByteArray &data);

    qint32  bytesToInt32(const QByteArray &bytes);

    QString ByteArrayToHexString(QByteArray ascii);

    bool sendAFrame(const QHostAddress &addr, quint16 port, const NetworkFrame &cmd);

    void parseUdpReceivedFrame(const QByteArray &data, const QHostAddress &peerAddr, const quint16 port);

    QUdpSocket      *m_udpClient;


    QHostAddress    m_localIpv4Addr;
    QHostAddress    m_serverIpv4Addr;

    quint16         m_serverPort;
    quint16         m_localPort;
};

#endif // NETWORKCLIENT_H
