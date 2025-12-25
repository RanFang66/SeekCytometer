#include "OpticsControlWidget.h"
#include <QGridLayout>
#include <QGroupBox>

OpticsControlWidget::OpticsControlWidget(const QString &tilte, QWidget *parent)
    : QDockWidget{tilte, parent}
{
    initDockWidget();
}

void OpticsControlWidget::initDockWidget()
{
    QGroupBox *groupLaser = new QGroupBox("Laser Control", this);
    QGroupBox *groupLed = new QGroupBox("LED Control", this);

    btnLaser_1 = new QPushButton("Laser-1", this);
    btnLaser_2 = new QPushButton("Laser-2", this);
    btnLed = new QPushButton("LED", this);

    spinLaserIntensity_1 = new QSpinBox(this);
    spinLaserIntensity_2 = new QSpinBox(this);
    spinLedIntensity = new QSpinBox(this);


    lblLaser1Status = new QLabel("Laser-1: Closed", this);
    lblLaser2Status = new QLabel("Laser-2: Closed", this);
    lblLedStatus = new QLabel("Led: Closed", this);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mainWidget);

    QGridLayout *laserLayout = new QGridLayout();
    laserLayout->addWidget(btnLaser_1, 0, 0);
    laserLayout->addWidget(new QLabel(tr("Laser1 Intensity Set:"), this), 0, 1);
    laserLayout->addWidget(spinLaserIntensity_1, 0, 2);
    laserLayout->addWidget(lblLaser1Status, 0, 3);

    laserLayout->addWidget(btnLaser_2, 1, 0);
    laserLayout->addWidget(new QLabel(tr("Laser2 Intensity Set:"), this), 1, 1);
    laserLayout->addWidget(spinLaserIntensity_2, 1, 2);
    laserLayout->addWidget(lblLaser2Status, 1, 3);

    QHBoxLayout *ledLayout = new QHBoxLayout();
    ledLayout->addWidget(btnLed);
    ledLayout->addWidget(new QLabel(tr("LED Intensity Set:"), this));
    ledLayout->addWidget(spinLedIntensity);
    ledLayout->addWidget(lblLedStatus);

    groupLaser->setLayout(laserLayout);
    groupLed->setLayout(ledLayout);
    layout->addWidget(groupLaser, 2);
    layout->addWidget(groupLed, 1);

    mainWidget->setLayout(layout);
    setWidget(mainWidget);
}
