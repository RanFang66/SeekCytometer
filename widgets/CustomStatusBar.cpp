#include "CustomStatusBar.h"
#include <QDateTime>
#include "NetworkClient.h"
#include "User.h"
CustomStatusBar::CustomStatusBar()
{
    initStatusBar();
}

void CustomStatusBar::updateConnectInfo(QString ip, quint16 port)
{
    lblConnectInfo->setText(tr("Connected, server Info: ") + QString("%1:%2").arg(ip).arg(port));
    connectLed->setStatus(StatusIndicator::STATUS_RUNNING);
}

void CustomStatusBar::initStatusBar()
{
    lblCurrTime = new QLabel(this);
    lblConnectInfo = new QLabel(tr("Not connected to server"), this);
    lblCurrentUser = new QLabel(User::loginUser().name(), this);
    connectLed = new StatusIndicator(this);


    addPermanentWidget(lblCurrTime);
    addPermanentWidget(lblCurrentUser);
    addWidget(connectLed);
    addWidget(lblConnectInfo);

    timerSecond = new QTimer(this);
    timerSecond->setInterval(1000);
    connect(timerSecond, &QTimer::timeout, this, [this](){
        lblCurrTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        if (connectLed->status() != StatusIndicator::STATUS_RUNNING) {
            NetworkClient::getInstance()->detectServer();
        }
    });

// #ifndef DEBUG_MODE
//     timerSecond->start();
// #endif
}
