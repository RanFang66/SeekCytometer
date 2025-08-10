#include "UdpCommClient.h"
#include "DataManager.h"
#include "DetectorSettingsModel.h"

UdpCommClient::UdpCommClient(QObject *parent)
    : QObject{parent}, m_udpSocket{new QUdpSocket(this)}, m_remotePort(0),
    m_sequenceCounter(0), m_sequenceValLast(0), m_sequenceReceived(0), m_sequenceReceivedLast(0),
    m_timerInterval(2000), m_commLostCounter(0), m_connected(false)
{
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpCommClient::onReadyRead);

    m_handshakeTimer = new QTimer();
    m_handshakeTimer->setInterval(2000);
    m_handshakeTimer->stop();

    connect(m_handshakeTimer, &QTimer::timeout, this, &UdpCommClient::onHandshakeTimerTimeout);

    m_remoteAddress = QHostAddress("192.168.8.10");
    m_remotePort = 5001;
    m_localAddress = QHostAddress("192.168.8.35");
    m_localPort = 8080;

}

void UdpCommClient::startUdpClient()
{
    m_handshakeTimer->start();
}


void UdpCommClient::doInitialize()
{
    bool bindOk = m_udpSocket->bind(m_localAddress, m_localPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!bindOk) {
        qWarning() << "[UdpCommClient] Failed to bind on"
                   << m_localAddress.toString() << ":" << m_localPort
                   << m_udpSocket->errorString();
    } else {
        qDebug() << "[UdpCommClient] Successfully bound on"
                 << m_localAddress.toString() << ":" << m_localPort;
    }
}


bool UdpCommClient::sendFrame(CommCmdType commandType, const QByteArray &data)
{
    QByteArray frame = UdpCommFrame::packFrame(++m_sequenceCounter, commandType, data);
    qint64 bytesSent = m_udpSocket->writeDatagram(frame, m_remoteAddress, m_remotePort);
    if (bytesSent != frame.size()) {
        qWarning() << "[UdpCommClient] Failed to send frame!"
                   << m_udpSocket->errorString();
        return false;
    }
    return true;
}

bool UdpCommClient::sendHandshake()
{
    return sendFrame(CommCmdType::CMD_HAND_SHAKE, QByteArray());
}

bool UdpCommClient::sendWaveformRequest(bool enabled, int enabledChannels, int interval)
{
    QByteArray data;

    // quint8 enableCh = 0;
    // for (int val : enabledChannels) {
    //     enableCh |=  (0x01 << (val - 1));
    // }

    data.append(static_cast<char>(enabled));
    data.append(static_cast<char>(enabledChannels & 0x00ff));
    data.append(static_cast<char>(interval & 0x00ff));

    return sendFrame(CommCmdType::CMD_WAVEFORM_DATA, data);
}

bool UdpCommClient::sendAcquireStart()
{
    return sendFrame(CommCmdType::CMD_ACQUIRE_START, QByteArray());
}

bool UdpCommClient::sendAcquireStop()
{
    return sendFrame(CommCmdType::CMD_ACQUIRE_STOP, QByteArray());
}

bool UdpCommClient::sendSortingStart()
{
    return sendFrame(CommCmdType::CMD_SORTING_START, QByteArray());
}

bool UdpCommClient::sendSortingStop()
{
    return sendFrame(CommCmdType::CMD_SORTING_STOP, QByteArray());
}

bool UdpCommClient::sendDetectorSettings(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                         const QList<int> &roles)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    for (const DetectorSettings &setting : DetectorSettingsModel::instance()->detectorSettings()) {
        stream << (char)setting.detectorId();
        stream << (char)setting.isEnabledHeight();
        stream << (char)setting.isEnabledWidth();
        stream << (char)setting.isEenabledArea();
        stream << (char)setting.isEnabledThreshold();
        stream << setting.thresholdValue();
    }
    if (!sendFrame(CommCmdType::CMD_DETECTOR_SETTINGS, data)) {
        return false;
    } else {
        qDebug() << "[UdpCommClient] Sent detector settings to remote.";
        return true;
    }
}

bool UdpCommClient::sendDisableDetector(int id)
{
    QByteArray data;
    data.append(static_cast<char>(id));
    if (!sendFrame(CommCmdType::CMD_DISABLE_DETECTOR, data)) {
        return false;
    } else {
        qDebug() << QString("[UdpCommClient] Disable detector %1 to remote.").arg(id);
        return true;
    }
}

void UdpCommClient::onHandshakeTimerTimeout()
{
    /*
     * If there is no frame send to SoC between this timer interval, then send handshake frame
     */
    if (m_sequenceValLast == m_sequenceCounter) {
        sendHandshake();
    }
    m_sequenceValLast = m_sequenceCounter;

    if (m_connected) {
        if (m_sequenceReceivedLast == m_sequenceReceived) {
            m_commLostCounter += m_timerInterval;
            if (m_commLostCounter > 10000) {
                emit udpCommLost();
                m_connected = false;
                m_commLostCounter = 0;
            }
        } else {
            m_commLostCounter = 0;
        }
    } else {
        if (m_sequenceReceivedLast != m_sequenceReceived) {
            emit udpCommEstablished();
            m_connected = true;
        }
    }

    m_sequenceReceivedLast = m_sequenceReceived;
}



void UdpCommClient::onReadyRead()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort = 0;
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        m_receiveBuffer.append(datagram);

        // Try to parse frames in a loop, because there may be multiple frames or partial frames
        while (true) {
            QByteArray oneFrame;
            if (!UdpCommFrame::tryParseFrame(m_receiveBuffer, oneFrame)) {
                break;
            }
            // We got a complete, valid frame
            quint16 sequence = UdpCommFrame::getSequence(oneFrame);
            m_sequenceReceived = sequence;

            CommCmdType cmdType = UdpCommFrame::getCommandType(oneFrame);
            QByteArray dataField = UdpCommFrame::getDataField(oneFrame);

            // Emit signal to notify listeners
            emit frameReceived(sequence, cmdType, dataField, sender, senderPort);
            switch (cmdType) {
                case CommCmdType::CMD_HAND_SHAKE:
                    parseHandshakeFrame(dataField);
                    break;
                case CommCmdType::CMD_PULSE_DATA:
                    parseSampleData(dataField);
                    break;
                case CommCmdType::CMD_WAVEFORM_DATA:
                    parseWaveformFrame(dataField);
                    break;
                default:
                    break;
            }
        }
    }
}

void UdpCommClient::parseHandshakeFrame(const QByteArray &data)
{
    QDataStream stream(data);
    quint8 state;
    stream >> state;
    qDebug() << "[UdpCommClient] Handshake frame received " << "Current Cytometer state:" << state;
}

void UdpCommClient::parseSampleData(const QByteArray &data)
{
    QVector<SampleData> sampleData;
    QDataStream stream(data);

    quint16 numSamples = 0;
    if (stream.atEnd()) {
        qWarning() << "[UdpCommClient] No sample data in the frame!";
        return;
    }

    stream >> numSamples;
    // Check data length is correct
    int expectedDataLength = numSamples * DataManager::instance().getSampleRecordByteSize();
    if (data.size() != expectedDataLength + sizeof(quint16)) {
        qWarning() << "[UdpCommClient] Incorrect sample data length!";
        return;
    }

    sampleData.reserve(numSamples);
    for (int i = 0; i < numSamples; ++i) {
        SampleData oneSample = DataManager::instance().getEmptySampleRecord();
        for (int j = 0; j < oneSample.size(); ++j) {
            for (int k = 0; k < oneSample.at(j).size(); ++k) {
                stream >> oneSample[j][k];
            }
        }
        sampleData.push_back(oneSample);
    }
    emit sampleDataReady(sampleData);
}


void UdpCommClient::parseWaveformFrame(const QByteArray &data)
{
    // QVector<QVector<int>> waveform;
    // QDataStream stream(data);
    // int numWaveforms = 0;
    // quint16 enabledChannels = 0;
    // stream >> numWaveforms;
    // stream >> enabledChannels;
    // QList<int> enabledChannelsList;
    // for (int i = 0; i < 8; ++i) {
    //     if (enabledChannels & (1 << i)) {
    //         enabledChannelsList.push_back(i);
    //     }
    // }
    // waveform.resize(numWaveforms);
    // for (int i = 0; i < numWaveforms; i++) {
    //     waveform[i].resize(enabledChannelsList.size());
    // }
    // int expectedDataLength = numWaveforms * enabledChannelsList.size() * sizeof(int);
    // if (data.size() != expectedDataLength + sizeof(int) + sizeof(quint16)) {
    //     qWarning() << "[UdpCommClient] Incorrect waveform data length!";
    //     return;
    // }

    // for (int i = 0; i < numWaveforms; i++) {
    //     for (int j = 0; j < enabledChannelsList.size(); j++) {
    //         stream >> waveform[i][j];
    //     }
    // }
    QVector<int> waveform;
    QDataStream stream(data);
    while (!stream.atEnd()) {
        int value;
        stream >> value;
        waveform.append(value);
    }

    emit waveformDataReceived(waveform);
}
