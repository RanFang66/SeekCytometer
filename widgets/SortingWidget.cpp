#include "SortingWidget.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>


SortingWidget::SortingWidget(const QString &tilte, QWidget *parent)
    : QDockWidget{tilte, parent}
{
    initSortingWidget();
}

void SortingWidget::initSortingWidget()
{
    QWidget *mainWidget = new QWidget(this);
    QGroupBox *groupDrive = new QGroupBox(tr("Sort Drive"), this);
    QGroupBox *groupSetup = new QGroupBox(tr("Sort Setup"), this);
    QGroupBox *groupStatus = new QGroupBox(tr("Sort Status"), this);


    btnRunSorting = new QPushButton(tr("Run"));
    btnPauseSorting = new QPushButton(tr("Pause"));
    editDriveWidth = new QLineEdit("100", this);
    // editDriveStrength = new QLineEdit("5000", this);
    editCoolingTime = new QLineEdit("200", this);
    editDriveDealy = new QLineEdit("100", this);
    comboDriveMode = new QComboBox(this);
    comboDriveMode->addItem(tr("Level Trig"));
    comboDriveMode->addItem(tr("Edge Trig"));
    // cBoxContinousMode = new QCheckBox(tr("Continous Mode"), this);
    comboSortMode = new QComboBox(this);
    comboSortMode->addItem(tr("Continous Mode"));
    comboSortMode->addItem(tr("Target Events"));
    editTargetEvents = new QLineEdit("10000", this);
    comboPopulation = new QComboBox(this);
    comboPopulation->addItem(tr("All"));


    QGridLayout *driveLayout = new QGridLayout(groupDrive);
    driveLayout->addWidget(new QLabel(tr("Drive Mode"), this), 0, 0);
    driveLayout->addWidget(comboDriveMode, 0, 1);

    driveLayout->addWidget(new QLabel(tr("Drive Width"), this), 0, 2);
    driveLayout->addWidget(editDriveWidth, 0, 3);
    // driveLayout->addWidget(new QLabel(tr("Drive Strength"), this), 0, 2);
    // driveLayout->addWidget(editDriveStrength, 0, 3);
    driveLayout->addWidget(new QLabel(tr("Cooling Time"), this), 1, 0);
    driveLayout->addWidget(editCoolingTime, 1, 1);
    driveLayout->addWidget(new QLabel(tr("Drive Delay"), this), 1, 2);
    driveLayout->addWidget(editDriveDealy, 1, 3);

    groupDrive->setLayout(driveLayout);

    QGridLayout *setupLayout = new QGridLayout(groupSetup);
    setupLayout->addWidget(btnRunSorting, 1, 0, 1, 3);
    setupLayout->addWidget(btnPauseSorting, 1, 3, 1, 2);
    setupLayout->addWidget(new QLabel(tr("Target Population"), this), 0, 0);
    setupLayout->addWidget(comboPopulation, 0, 1);
    // setupLayout->addWidget(cBoxContinousMode, 1, 2);
    setupLayout->addWidget(comboSortMode, 0, 2);
    setupLayout->addWidget(new QLabel(tr("Target Events"), this), 0, 3);
    setupLayout->addWidget(editTargetEvents, 0, 4);
    groupSetup->setLayout(setupLayout);


    QGridLayout *statusLayout = new QGridLayout(groupStatus);
    sortNum = new QLabel("0", this);
    abortNum = new QLabel("0", this);
    sortRate = new QLabel("0", this);
    abortRate = new QLabel("0", this);
    sortEfficiency = new QLabel("0", this);
    progressSort = new QProgressBar(this);
    statusLayout->addWidget(new QLabel(tr("Sort Number"), this), 0, 0);
    statusLayout->addWidget(sortNum, 0, 1);
    statusLayout->addWidget(new QLabel(tr("Abort Number"), this), 0, 2);
    statusLayout->addWidget(abortNum, 0, 3);
    statusLayout->addWidget(new QLabel(tr("Sort Rate"), this), 1, 0);
    statusLayout->addWidget(sortRate, 1, 1);
    statusLayout->addWidget(new QLabel(tr("Abort Rate"), this), 1, 2);
    statusLayout->addWidget(abortRate, 1, 3);
    statusLayout->addWidget(new QLabel(tr("Sort Efficiency"), this), 2, 0);
    statusLayout->addWidget(sortEfficiency, 2, 1);
    statusLayout->addWidget(progressSort, 2, 2, 1, 2);
    groupStatus->setLayout(statusLayout);




    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->addWidget(groupDrive);
    mainLayout->addWidget(groupSetup);
    mainLayout->addWidget(groupStatus);
    mainWidget->setLayout(mainLayout);

    setWidget(mainWidget);
}
