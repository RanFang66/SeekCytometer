#include "SampleChipWidget.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

SampleChipWidget::SampleChipWidget(const QString &tilte, QWidget *parent)
    : QDockWidget{tilte, parent}
{
    initSampleChipWidget();
}

void SampleChipWidget::initSampleChipWidget()
{
    QGroupBox *groupChipMove = new QGroupBox("Chip Move", this);
    QGroupBox *groupLenMove = new QGroupBox("Len Move", this);
    QGroupBox *groupCover = new QGroupBox("Cover Control", this);
    QGroupBox *groupSeal = new QGroupBox("Seal Control", this);
    QGroupBox *groupChurn = new QGroupBox("Churn Control", this);
    QGroupBox *groupTempControl = new QGroupBox("Temperature Control", this);




    btnMoveXBackward = new QPushButton("Left", this);
    btnMoveXForward = new QPushButton("Right", this);
    btnMoveYBackward = new QPushButton("Backward", this);
    btnMoveYForward = new QPushButton("Forward", this);

    btnMoveLenUp = new QPushButton("Up", this);
    btnMoveLenDown = new QPushButton("Down", this);

    btnOpenCover = new QPushButton("Open Cover", this);
    btnCloseCover = new QPushButton("Close Cover", this);

    btnPressSample = new QPushButton("Press Sample", this);
    btnReleaseSample = new QPushButton("Release Sample", this);

    btnChurnRunCW = new QPushButton("Churn Clockwise", this);
    btnChurnRunCCW = new QPushButton("Churn Counter Clockwise", this);
    btnTempControl = new QPushButton("Temp Control", this);

    lblCoverStatus = new QLabel("Cover: Opened", this);
    lblPressStatus = new QLabel("Presser: Released", this);
    lblChurnStatus = new QLabel("Churn: IDLE", this);
    lblTempContorlStatus = new QLabel("Current: 25 ℃", this);
    lblChipPos = new QLabel("Chip Position: X: 100, Y: 10000", this);
    lblLenPos = new QLabel("Len(Z) Pos: 10000", this);

    spinChurnSpeed = new QSpinBox(this);
    spinChurnSpeed->setMinimum(10);
    spinChurnSpeed->setMaximum(2000);
    spinChurnSpeed->setValue(100);

    spinTargetTemp = new QSpinBox(this);
    spinTargetTemp->setMaximum(1);
    spinTargetTemp->setMaximum(40);
    spinTargetTemp->setValue(25);



    QHBoxLayout *coverLayout = new QHBoxLayout();
    coverLayout->addWidget(btnOpenCover);
    coverLayout->addWidget(btnCloseCover);
    coverLayout->addWidget(lblCoverStatus);

    QHBoxLayout *sealLayout = new QHBoxLayout();
    sealLayout->addWidget(btnPressSample);
    sealLayout->addWidget(btnReleaseSample);
    sealLayout->addWidget(lblPressStatus);


    QHBoxLayout *churnLayout = new QHBoxLayout();
    churnLayout->addWidget(btnChurnRunCW);
    churnLayout->addWidget(btnChurnRunCCW);
    churnLayout->addWidget(spinChurnSpeed);
    churnLayout->addWidget(lblChurnStatus);


    QGridLayout *tempControlLayout = new QGridLayout();
    tempControlLayout->addWidget(new QLabel(tr("Target Temp(℃):"), this), 0, 0);
    tempControlLayout->addWidget(spinTargetTemp, 0, 1);
    tempControlLayout->addWidget(btnTempControl, 0, 2);
    tempControlLayout->addWidget(new QLabel(tr("Current Temp(℃):"), this), 1, 0);
    tempControlLayout->addWidget(lblTempContorlStatus, 1, 1, 1, 2);

    QGridLayout *chipMoveLayout = new QGridLayout();
    chipMoveLayout->addWidget(btnMoveYForward, 0, 1);
    chipMoveLayout->addWidget(btnMoveXBackward, 1, 0);
    chipMoveLayout->addWidget(lblChipPos, 1, 1);
    chipMoveLayout->addWidget(btnMoveXForward, 1, 2);
    chipMoveLayout->addWidget(btnMoveYBackward, 2, 1);


    QVBoxLayout *lenMoveLayout = new QVBoxLayout();
    lenMoveLayout->addWidget(btnMoveLenUp);
    lenMoveLayout->addWidget(lblLenPos);
    lenMoveLayout->addWidget(btnMoveLenDown);



    groupChipMove->setLayout(chipMoveLayout);
    groupLenMove->setLayout(lenMoveLayout);
    groupChurn->setLayout(churnLayout);
    groupCover->setLayout(coverLayout);
    groupSeal->setLayout(sealLayout);
    groupTempControl->setLayout(tempControlLayout);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    QHBoxLayout *moveLayout = new QHBoxLayout();
    moveLayout->addWidget(groupChipMove);
    moveLayout->addWidget(groupLenMove);
    mainLayout->addWidget(groupCover, 1);
    mainLayout->addWidget(groupSeal, 1);
    mainLayout->addWidget(groupChurn, 1);
    mainLayout->addWidget(groupTempControl, 2);
    mainLayout->addLayout(moveLayout, 3);

    setWidget(mainWidget);
}
