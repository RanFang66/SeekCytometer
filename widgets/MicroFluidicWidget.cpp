#include "MicroFluidicWidget.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>

MicroFluidicWidget::MicroFluidicWidget(const QString &tilte, QWidget *parent)
    : QDockWidget{tilte, parent}
{
    initDockWidget();
}

void MicroFluidicWidget::initDockWidget()
{
    QGridLayout *debugLayout = new QGridLayout();
    QGroupBox *groupBoxDebug = new QGroupBox("Device Debug", this);

    QGridLayout *controlLayout = new QGridLayout();
    QGroupBox *groupBoxControl = new QGroupBox("Press Control", this);

    for (int i = 0; i < CHANNEL_NUM; i++) {
        btnSoleValve[i] = new QPushButton(QString("SoleValve-CH%1").arg(i+1), this);
        btnPressControl[i] = new QPushButton(QString("Enable Control-CH%1").arg(i+1), this);


        lblPress[i] = new QLabel(QString("CH%1 Press: ").arg(i+1), this);
        lblPressControl[i] = new QLabel(QString("CH%1 Press: ").arg(i+1), this);

        spinTargetPress[i] = new QSpinBox(this);
        spinTargetPress[i]->setRange(0, 1000);
        spinTargetPress[i]->setValue(0);

        spinPropoValveValue[i] = new QSpinBox(this);
        spinPropoValveValue[i]->setRange(0, 65535);
        spinPropoValveValue[i]->setValue(0);

        debugLayout->addWidget(new QLabel(QString("Ch-%1").arg(i+1), this), i, 0);
        debugLayout->addWidget(btnSoleValve[i], i, 1);
        debugLayout->addWidget(spinPropoValveValue[i], i, 2);
        debugLayout->addWidget(lblPress[i], i, 3);

        controlLayout->addWidget(new QLabel(QString("Ch-%1 Target Press").arg(i+1), this), i, 0);
        controlLayout->addWidget(spinTargetPress[i], i, 1);
        controlLayout->addWidget(btnPressControl[i], i, 2);
        controlLayout->addWidget(lblPressControl[i], i, 3);
    }
    lblSourePress = new QLabel(QString("Source Press: 1000 mbar"), this);
    groupBoxDebug->setLayout(debugLayout);
    groupBoxControl->setLayout(controlLayout);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mainWidget);
    layout->addWidget(lblSourePress, 1);
    layout->addWidget(groupBoxDebug, 4);
    layout->addWidget(groupBoxControl, 4);

    mainWidget->setLayout(layout);
    setWidget(mainWidget);
}
