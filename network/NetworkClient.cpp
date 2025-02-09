#include "NetworkClient.h"


NetworkClient::NetworkClient(QObject *parent)
    : QObject{parent},
    m_udpClient(new QUdpSocket(this)),
    m_localPort(8080)
{
    initNetworkClient();

}

const QString NetworkClient::getLocalIpv4() const
{
    QString ipv4Addr;
    QList<QNetworkInterface> allInterface = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : allInterface) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp)
            && interface.flags().testFlag(QNetworkInterface::IsRunning)
            && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            if (interface.type() == QNetworkInterface::Ethernet) {
                QList<QNetworkAddressEntry> allEntries = interface.addressEntries();
                for (const QNetworkAddressEntry &entry: allEntries) {
                    if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                        ipv4Addr = entry.ip().toString();
                        qDebug() << "Local Ethernet IP Address: " << entry.ip().toString();
                    }
                }
            }
        }
    }
    return ipv4Addr;
}

void NetworkClient::detectServer()
{
    NetworkFrame detectFrame(NetworkFrame::DETECT_SERVER, NetworkFrame::CH_0, QByteArray());
    sendAFrame(QHostAddress::Broadcast, m_serverPort, detectFrame);
}

void NetworkClient::getWaveform(int sampleCh, int sampleLen)
{
    QByteArray data;
    data.resize(8);
    qToBigEndian(sampleCh, reinterpret_cast<uchar*>(data.data()));
    qToBigEndian(sampleLen, reinterpret_cast<uchar*>(data.data() + 4));
    NetworkFrame waveformFrame(NetworkFrame::GET_CH_WAVEFORM, NetworkFrame::CH_0, data);
    sendAFrame(m_serverIpv4Addr, m_serverPort, waveformFrame);
}

void NetworkClient::setChannelSetting(NetworkFrame::CHANNEL_INDEX channel)
{
    QByteArray data;
    data.resize(9);
    // const ChannelSettingModel::ChannelSetting &setting = CurrentSettings::getSettings().getChannelSettings(channel);
    // data[0] = setting.enabled ? 1 : 0;
    // qToBigEndian(setting.validThresh, reinterpret_cast<uchar*>(data.data() + 1));
    // qToBigEndian(setting.sortThresh, reinterpret_cast<uchar*>(data.data() + 5));

    // NetworkFrame threshFrame(NetworkFrame::SET_CH_THRESH, static_cast<NetworkFrame::CHANNEL_INDEX>(channel), data);
    // sendAFrame(m_serverIpv4Addr, m_serverPort, threshFrame);
}

void NetworkClient::setAnalyzerState(bool enable)
{
    QByteArray data;
    char en = enable ? 1:0;
    data.append(en);
    NetworkFrame configFrame(NetworkFrame::CONFIG_ANALYZER, NetworkFrame::CH_0, data);
    sendAFrame(m_serverIpv4Addr, m_serverPort, configFrame);
}

void NetworkClient::setDriveSetting()
{
    QByteArray data;
    data.resize(13);
    // const DriverSettingModel::DriveSetting &setting = CurrentSettings::getSettings().getDriveSettings();
    // if (setting.driveTrigType == DriverSettingModel::DriveLevelTrig) {
    //     data[0] = 0;
    // } else {
    //     data[0] = 1;
    // }
    // qToBigEndian(setting.driveDelay, reinterpret_cast<uchar*>(data.data()+1));
    // qToBigEndian(setting.driveWidth, reinterpret_cast<uchar*>(data.data() + 5));
    // qToBigEndian(setting.coolingTime, reinterpret_cast<uchar*>(data.data() + 9));

    // NetworkFrame driveParaFrame(NetworkFrame::SET_DRIVE_PARAS, NetworkFrame::CH_0, data);
    // sendAFrame(m_serverIpv4Addr, m_serverPort, driveParaFrame);
}

QString NetworkClient::getUdpClientState()
{
    QString state;

    switch (m_udpClient->state()) {
    case QAbstractSocket::UnconnectedState:
        state = "UnconnectedState";
        break;
    case QAbstractSocket::HostLookupState:
        state = "HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        state = "ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        state = "ConnectedState";
        break;
    case QAbstractSocket::BoundState:
        state = "BoundState";
        break;
    case QAbstractSocket::ClosingState:
        state = "ClosingState";
        break;
    case QAbstractSocket::ListeningState:
        state = "ListeningState";
    }
    return state;
}

// void NetworkClient::onUdpSocketStateChanged(QAbstractSocket::SocketState socketState)
// {
//     switch (socketState) {
//     case QAbstractSocket::UnconnectedState:
//         m_socketState = "UnconnectedState";
//         break;
//     case QAbstractSocket::HostLookupState:
//         m_socketState = "HostLookupState";
//         break;
//     case QAbstractSocket::ConnectingState:
//         m_socketState = "ConnectingState";
//         break;
//     case QAbstractSocket::ConnectedState:
//         m_socketState = "ConnectedState";
//         break;
//     case QAbstractSocket::BoundState:
//         m_socketState = "BoundState";
//         break;
//     case QAbstractSocket::ClosingState:
//         m_socketState = "ClosingState";
//         break;
//     case QAbstractSocket::ListeningState:
//         m_socketState = "ListeningState";
//     }
// }

void NetworkClient::onUdpSocketReadyRead()
{
    QHostAddress peerAddr;
    quint16 peerPort;
    while(m_udpClient->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpClient->pendingDatagramSize());
        qint64 readLen = m_udpClient->readDatagram(datagram.data(), datagram.size(), &peerAddr, &peerPort);
        qDebug() << QString("[From %1 : %2](%3 bytes)").arg(peerAddr.toIPv4Address()).arg(peerPort).arg(readLen);
        if (peerAddr.isBroadcast() || peerAddr.toIPv4Address() == m_localIpv4Addr.toIPv4Address()) {
            qDebug() << QString("Received a broadcast datagram: ") << datagram;
        } else {
            parseUdpReceivedFrame(datagram, peerAddr, peerPort);
        }
    }
}

void NetworkClient::initNetworkClient()
{
    QList<QNetworkInterface> allInterface = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : allInterface) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp)
            && interface.flags().testFlag(QNetworkInterface::IsRunning)
            && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            if (interface.type() == QNetworkInterface::Ethernet) {
                QList<QNetworkAddressEntry> allEntries = interface.addressEntries();
                for (const QNetworkAddressEntry &entry: allEntries) {
                    if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                        m_localIpv4Addr = entry.ip();
                        qDebug() << "Local Ethernet IP Address: " << entry.ip().toString();
                    }
                }
            }
        }
    }
    if (m_localIpv4Addr.isNull()) {
        m_localIpv4Addr = QHostAddress("192.168.1.35");
        qDebug() << "Can not find valid Ethernet!";
    }
    m_serverIpv4Addr = QHostAddress("192.168.1.10");
    m_serverPort = 8080;
    m_udpClient->bind(m_localPort);
    // connect(m_udpClient, &QUdpSocket::stateChanged, this, &NetworkClient::onUdpSocketStateChanged);
    connect(m_udpClient, &QUdpSocket::readyRead, this, &NetworkClient::onUdpSocketReadyRead);
}

quint8 NetworkClient::checkSum(const QByteArray &data)
{
    quint8 chksum = 0;
    for (const char ch : data) {
        chksum += ch;
    }
    return chksum;
}

qint32 NetworkClient::bytesToInt32(const QByteArray &bytes)
{
    qint32 num = 0;
    qFromBigEndian<qint32>(bytes, bytes.length(), &num);
    return num;
}

QString NetworkClient::ByteArrayToHexString(QByteArray ascii)
{
    QString ret;
    for(int i = 0; i < ascii.length(); i++)
        ret.append(QString("%1 ").arg((uchar)ascii.at(i), 2, 16, (QChar)'0'));

    return ret.toUpper();
}


bool NetworkClient::sendAFrame(const QHostAddress &addr, quint16 port, const NetworkFrame &cmd)
{
    qint64 sendLength = m_udpClient->writeDatagram(cmd.getSendBuff(), addr, port);
    return (sendLength == cmd.getSendBuff().length());
}

void NetworkClient::parseUdpReceivedFrame(const QByteArray &data, const QHostAddress &peerAddr, const quint16 port)
{
    if (data.length() < 8) {
        qDebug() << "Parse failed: Datagram is too short!";
        return;
    }
   if ((data.at(0) & 0x0001)  == 0) {
       qDebug() << "Parse failed: Not a valid frame header!";
       return;
    }

    quint16 cmdType = ((quint16)data.at(1) << 8 | (quint16)data.at(2));
    quint16 channel = ((quint16)data.at(3) << 8 | (quint16)data.at(4));
    quint16 dataLen = ((quint16)data.at(5) << 8 | (quint16)data.at(6));

    if (dataLen + 8 != data.length()) {
        qDebug() << "Parse failed: data length do not match the frame length";
        return;
    }
    QByteArray frameData(data.mid(7, dataLen));
    switch (cmdType) {
    case NetworkFrame::DETECT_SERVER: {
        if (frameData.length() < 16)
            return;
        QString macAddress = ByteArrayToHexString(data.mid(7, 6));
        // int     ipInt = bytesToInt32(data.mid(13, 4));
        int     sampleChannelNum = data.at(17);
        int     sampleDepth = data.at(18);
        qint32  sampleFreq = bytesToInt32(data.mid(19, 4));

        m_serverIpv4Addr = QHostAddress(peerAddr.toIPv4Address());
        m_serverPort = port;
        qDebug() << QString("Get info frame: macAddress[%1], ip[%2], sampleDepth[%3], sampleRate[%4]")
                        .arg(macAddress).arg(m_serverIpv4Addr.toString()).arg(sampleDepth).arg(sampleFreq);
        emit receivedServerInfoFrame(m_serverIpv4Addr.toString(), port);
        break;
    }
    case NetworkFrame::GET_CH_WAVEFORM: {
        QList<qreal> waveData;
        for (qsizetype i = 0; i < dataLen / 4; i++) {
            int val;
            val = bytesToInt32(frameData.mid(i*4, 4));
            //qFromBigEndian<int>(frameData.mid(i*4, 4), 4, &val);
            qreal voltage = val;
            waveData.append(voltage);
        }
        qDebug() << "Receive waveform data : " << channel << (int)frameData.at(0) << (int)frameData.at(1) << (int)frameData.at(2) << (int)frameData.at(3);
        emit receivedWaveFormFrame(channel, waveData);
        break;
    }
    case NetworkFrame::GET_CH_PULSE_RES: {
        qDebug() << QString("Get pulse frame of channel-%1: data length: %2").arg(channel).arg(data.length()-8);
        emit receivedPulseResFrame(frameData);
        // PulseDataManage::getInstance()->insertPulseData(frameData);

        // QList<qreal> pulseData;

        // for (qsizetype i = 12; i < dataLen; i+=16) {
        //     int val = (((int)frameData.at(i) << 24) + ((int)frameData.at(i+1) << 16) + ((int)frameData.at(i+2)<< 8) + ((int)frameData.at(i+3)));
        //     // qFromBigEndian<int>(frameData.mid(i, 4), 4, &val);
        //     qreal pulseVal = val / 32768.0 * 5000.0;
        //     pulseData.append(pulseVal);
        // }
        // emit receivedPulseValue(channel, pulseData);
        break;
    }
    case NetworkFrame::SET_CH_THRESH: {
        int validThresh = bytesToInt32(frameData.mid(0, 4));
        int sortThresh = bytesToInt32(frameData.mid(4, 4));
        emit receivedPulseThreshFrame(channel, validThresh, sortThresh);
        break;
    }
    case NetworkFrame::SET_DRIVE_PARAS: {
        int driveDelay = bytesToInt32(frameData.mid(0, 4));
        int driveWidth = bytesToInt32(frameData.mid(4, 4));
        int coolTime = bytesToInt32(frameData.mid(8, 4));
        int sortChEn = bytesToInt32(frameData.mid(12, 4));
        emit receivedDriveParametersFrame(driveDelay, driveWidth, coolTime, sortChEn);
        break;
    }
    default:
        break;
    }
}


