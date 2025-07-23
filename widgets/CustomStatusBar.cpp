#include "CustomStatusBar.h"
#include <QDateTime>
#include "User.h"
CustomStatusBar::CustomStatusBar()
{
    initStatusBar();
}

void CustomStatusBar::updateConnectInfo(StatusIndicator::STATUS status, const QString &info)
{
    // lblConnectInfo->setText(tr("Connected, server Info: ") + QString("%1:%2").arg(ip).arg(port));
    lblConnectInfo->setText(info);
    connectLed->setStatus(status);
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
    });

    #ifndef DEBUG_MODE
        timerSecond->start();
    #endif
}
