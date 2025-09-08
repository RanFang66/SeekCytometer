#ifndef CYTOMETERCONTROLLER_H
#define CYTOMETERCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include "UdpCommClient.h"
#include <QThread>

class CytometerController : public QObject
{
    Q_OBJECT
public:
    static CytometerController *instance()
    {
        static CytometerController instance;
        return &instance;
    }
    void start();
    ~CytometerController();

public slots:
    void zynqConnect();
    void zynqDisconnect();
    void startAcquisition();
    void stopAcquisition();
    void startSorting();
    void stopSorting();
    void reset();
    void triggerError();

signals:
    void connected();
    void disconnected();
    void acquisitionStarted();
    void acquisitionStopped();
    void sortingStarted();
    void sortingStopped();
    void errorOccurred();
    void errorResolved();


private slots:
    void onEnterAccquiringState();
    void onEnterSortingState();
    void onExitAccquiringState();
    void onExitSortingState();
    void onEnterErrorState();
    void onExitErrorState();


    // void initUdpClient();

private:
    explicit CytometerController(QObject *parent = nullptr);
    CytometerController(const CytometerController &) = delete;
    CytometerController &operator=(const CytometerController &) = delete;

    QStateMachine m_stateMachine;

    UdpCommClient   *m_udpClient;
    QThread         *m_udpClientThread;

    QState *unconnectedState;
    QState *idleState;
    QState *acquiringState;
    QState *sortingState;
    QState *errorState;

};

#endif // CYTOMETERCONTROLLER_H
