#ifndef CUSTOMSTATUSBAR_H
#define CUSTOMSTATUSBAR_H

#include <QStatusBar>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "StatusIndicator.h"

class CustomStatusBar : public QStatusBar
{
    Q_OBJECT
public:
    CustomStatusBar();

public slots:
    void updateConnectInfo(StatusIndicator::STATUS  status, const QString &info);


private:
    QLabel *lblCurrTime;
    QLabel *lblConnectInfo;
    QLabel *lblCurrentUser;
    QTimer *timerSecond;
    StatusIndicator *connectLed;
    void initStatusBar();
};

#endif // CUSTOMSTATUSBAR_H
