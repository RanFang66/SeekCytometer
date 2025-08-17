#include "CytometerController.h"
#include <QDebug>
#include "DataManager.h"
#include "DetectorSettingsModel.h"
#include "WorkSheetWidget.h"
#include "TestDataGenerator.h"
#include "SortingWidget.h"
#include "WaveformWidget.h"


CytometerController::CytometerController(QObject *parent)
    : QObject{parent}, m_udpClientThread(new QThread(this)), m_udpClient(new UdpCommClient())
{
    m_udpClient->moveToThread(m_udpClientThread);

    QObject::connect(m_udpClientThread, &QThread::started, m_udpClient, &UdpCommClient::doInitialize);


    unconnectedState = new QState(&m_stateMachine);
    idleState = new QState(&m_stateMachine);
    acquiringState = new QState(&m_stateMachine);
    sortingState = new QState(&m_stateMachine);
    errorState = new QState(&m_stateMachine);

    unconnectedState->assignProperty(this, "status", "Unconnected");
    idleState->assignProperty(this, "status", "Idle");
    acquiringState->assignProperty(this, "status", "Acquiring");
    sortingState->assignProperty(this, "status", "Sorting");
    errorState->assignProperty(this, "status", "Error");

    QObject::connect(unconnectedState, &QState::entered, this, [this](){qDebug() << "Entering Unconnected state";});
    QObject::connect(idleState, &QState::entered, this, [this](){qDebug() << "Entering Idle State";});
    QObject::connect(acquiringState, &QState::entered, this, &CytometerController::onEnterAccquiringState);
    QObject::connect(sortingState, &QState::entered, this, &CytometerController::onEnterSortingState);
    QObject::connect(errorState, &QState::entered, this, &CytometerController::onEnterErrorState);
    QObject::connect(acquiringState, &QState::exited, this, &CytometerController::onExitAccquiringState);
    QObject::connect(sortingState, &QState::exited, this, &CytometerController::onExitSortingState);
    QObject::connect(errorState, &QState::exited, this, &CytometerController::onExitErrorState);




    QObject::connect(m_udpClient, &UdpCommClient::udpCommEstablished, this, &CytometerController::connect);
    QObject::connect(m_udpClient, &UdpCommClient::udpCommLost, this, &CytometerController::disconnect);
    QObject::connect(this, &CytometerController::acquisitionStarted, m_udpClient, &UdpCommClient::sendAcquireStart);
    QObject::connect(this, &CytometerController::acquisitionStopped, m_udpClient, &UdpCommClient::sendAcquireStop);
    QObject::connect(this, &CytometerController::sortingStarted, m_udpClient, &UdpCommClient::sendSortingStart);
    QObject::connect(this, &CytometerController::sortingStopped, m_udpClient, &UdpCommClient::sendSortingStop);

    QObject::connect(DetectorSettingsModel::instance(), &DetectorSettingsModel::dataChanged, m_udpClient, &UdpCommClient::sendDetectorSettings);
    QObject::connect(SortingWidget::instance(), &SortingWidget::driveParametersChanged, m_udpClient, &UdpCommClient::sendDriveParameters);
    QObject::connect(SortingWidget::instance(), &SortingWidget::gateChanged, m_udpClient, &UdpCommClient::sendGateData);


    QObject::connect(WaveformWidget::instance(), &WaveformWidget::waveformStateChanged, m_udpClient, &UdpCommClient::sendWaveformRequest);
    QObject::connect(m_udpClient, &UdpCommClient::waveformDataReceived, WaveformWidget::instance(), &WaveformWidget::onReceivedWaveform);


    unconnectedState->addTransition(this, &CytometerController::connected, idleState);
    idleState->addTransition(this, &CytometerController::acquisitionStarted, acquiringState);
    idleState->addTransition(this, &CytometerController::sortingStarted, sortingState);
    idleState->addTransition(this, &CytometerController::disconnected, unconnectedState);
    acquiringState->addTransition(this, &CytometerController::acquisitionStopped, idleState);
    acquiringState->addTransition(this, &CytometerController::disconnected, unconnectedState);
    sortingState->addTransition(this, &CytometerController::sortingStopped, idleState);
    sortingState->addTransition(this, &CytometerController::disconnected, unconnectedState);
    errorState->addTransition(this, &CytometerController::errorResolved, idleState);
    idleState->addTransition(this, &CytometerController::errorOccurred, errorState);
    acquiringState->addTransition(this, &CytometerController::errorOccurred, errorState);
    sortingState->addTransition(this, &CytometerController::errorOccurred, errorState);

    m_stateMachine.setInitialState(unconnectedState);
    m_udpClientThread->start();
}


void CytometerController::start()
{
    m_stateMachine.start();
    m_udpClient->startUdpClient();
}

CytometerController::~CytometerController()
{
    m_udpClientThread->quit();
    m_udpClientThread->wait();

    if (m_udpClient) {
        m_udpClient->deleteLater();
        m_udpClient = nullptr;
    }
}

void CytometerController::connect()
{
    emit connected();
}

void CytometerController::disconnect()
{
    emit disconnected();
}

void CytometerController::startAcquisition()
{
    emit acquisitionStarted();
}

void CytometerController::stopAcquisition()
{
    emit acquisitionStopped();
}

void CytometerController::startSorting()
{
    emit sortingStarted();
}

void CytometerController::stopSorting()
{
    emit sortingStopped();
}

void CytometerController::reset()
{
    emit disconnected();
}

void CytometerController::triggerError()
{
    emit errorOccurred();
}


void CytometerController::onEnterAccquiringState()
{
    qDebug() << "Entering Acquiring State";

    DataManager::instance().initDataManager(DetectorSettingsModel::instance()->detectorSettings());

#if ENABLE_DEBUG
    TestDataGenerator::instance().configTestGenerator(50, 200, 3000, 80000);
    QObject::connect(&TestDataGenerator::instance(), &TestDataGenerator::testDataGenerated, &DataManager::instance(), &DataManager::addSamples);
    TestDataGenerator::instance().startGenerateData();
#else
    QObject::connect(m_udpClient, &UdpCommClient::sampleDataReady, &DataManager::instance(), &DataManager::addSamples);
#endif
    WorkSheetWidget::instance()->setActive(true);
}

void CytometerController::onExitAccquiringState()
{
    qDebug() << "Exiting Acquiring State";

#if ENABLE_DEBUG
    TestDataGenerator::instance().stopGenerateData();
    QObject::disconnect(&TestDataGenerator::instance(), &TestDataGenerator::testDataGenerated, &DataManager::instance(), &DataManager::addSamples);
#else
    QObject::disconnect(m_udpClient, &UdpCommClient::sampleDataReady, &DataManager::instance(), &DataManager::addSamples);
#endif
    WorkSheetWidget::instance()->setActive(false);
}

void CytometerController::onEnterSortingState()
{
    qDebug() << "Entering Sorting State";

    DataManager::instance().initDataManager(DetectorSettingsModel::instance()->detectorSettings());
#if ENABLE_DEBUG
    TestDataGenerator::instance().configTestGenerator(50, 200, 0, 131072);
    QObject::connect(&TestDataGenerator::instance(), &TestDataGenerator::testDataGenerated, &DataManager::instance(), &DataManager::addSamples);
    TestDataGenerator::instance().startGenerateData();
#else
    QObject::connect(m_udpClient, &UdpCommClient::sampleDataReady, &DataManager::instance(), &DataManager::addSamples);
#endif
    WorkSheetWidget::instance()->setActive(true);

}

void CytometerController::onExitSortingState()
{
    qDebug() << "Exiting Sorting State";

#if ENABLE_DEBUG
    TestDataGenerator::instance().stopGenerateData();
    QObject::disconnect(&TestDataGenerator::instance(), &TestDataGenerator::testDataGenerated, &DataManager::instance(), &DataManager::addSamples);
#else
    QObject::disconnect(m_udpClient, &UdpCommClient::sampleDataReady, &DataManager::instance(), &DataManager::addSamples);
#endif
    WorkSheetWidget::instance()->setActive(false);

}

void CytometerController::onEnterErrorState()
{
    qDebug() << "Entering Error State";
#if ENABLE_DEBUG
    TestDataGenerator::instance().stopGenerateData();
    QObject::disconnect(&TestDataGenerator::instance(), &TestDataGenerator::testDataGenerated, &DataManager::instance(), &DataManager::addSamples);
    WorkSheetWidget::instance()->setActive(false);
#endif
}

void CytometerController::onExitErrorState()
{
    qDebug() << "Exiting Error State";

#if ENABLE_DEBUG
    TestDataGenerator::instance().configTestGenerator(100, 500, 0, 32768);
    QObject::connect(&TestDataGenerator::instance(), &TestDataGenerator::testDataGenerated, &DataManager::instance(), &DataManager::addSamples);
    TestDataGenerator::instance().startGenerateData();
#endif
}

// void CytometerController::initUdpClient()
// {
//     QHostAddress localAddress("192.168.1.35");
//     quint16      localPort = 8080;
//     QHostAddress serverAddress("192.168.1.10");
//     quint16      serverPort = 8080;

//     QMetaObject::invokeMethod(m_udpClient, [=](){
//         m_udpClient->doInitialize(localAddress, localPort, serverAddress, serverPort);
//     }, Qt::QueuedConnection);
// }







