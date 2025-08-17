#include "SortingWidget.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <GatesDAO.h>
#include <DetectorSettingsDAO.h>
#include "CytometerController.h"

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


    btnRunSorting = new QPushButton(tr("Start Sort"));
    btnPauseSorting = new QPushButton(tr("Pause"));
    editDriveWidth = new QLineEdit("100", this);
    // editDriveStrength = new QLineEdit("5000", this);
    editCoolingTime = new QLineEdit("200", this);
    editDriveDealy = new QLineEdit("100", this);
    comboDriveMode = new QComboBox(this);
    comboDriveMode->addItem(tr("Level Trig"), 0);
    comboDriveMode->addItem(tr("Edge Trig"), 1);
    // cBoxContinousMode = new QCheckBox(tr("Continous Mode"), this);
    comboSortMode = new QComboBox(this);
    comboSortMode->addItem(tr("Continous Mode"));
    comboSortMode->addItem(tr("Target Events"));
    editTargetEvents = new QLineEdit("10000", this);
    editTargetEvents->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    comboPopulation = new QComboBox(this);
    comboPopulation->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    comboPopulation->addItem(tr("All"),0);


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


    connect(btnRunSorting, &QPushButton::clicked, this, &SortingWidget::startSorting);
    connect(comboDriveMode, &QComboBox::currentIndexChanged, this, &SortingWidget::changeDriveParameters);
    connect(editCoolingTime, &QLineEdit::editingFinished, this, &SortingWidget::changeDriveParameters);
    connect(editDriveWidth, &QLineEdit::editingFinished, this, &SortingWidget::changeDriveParameters);
    connect(editDriveDealy, &QLineEdit::editingFinished, this, &SortingWidget::changeDriveParameters);

    connect(comboPopulation, &QComboBox::currentIndexChanged, this, &SortingWidget::changeGate);
}

void SortingWidget::updatePopulation(int workSheetId)
{
    comboPopulation->clear();
    QList<Gate> gateList = GatesDAO().fetchGates(workSheetId);
    for (const Gate &gate : gateList) {
        comboPopulation->addItem(gate.name(), gate.id());
    }
    comboPopulation->addItem("All", 0);
}

const Gate SortingWidget::getCurrentPopulation() const
{
    int id = comboPopulation->currentData().toInt();

    if (id == 0) {
        return Gate();
    }
    return GatesDAO().fetchGate(id);
}

void SortingWidget::changeDriveParameters()
{
    m_driveType = comboDriveMode->currentData().toInt();
    m_driveDelay = editDriveDealy->text().toInt();
    m_driveWidth = editDriveWidth->text().toInt();
    m_coolingTime = editCoolingTime->text().toInt();

    emit driveParametersChanged(m_driveType, m_driveDelay, m_driveWidth, m_coolingTime);
}

void SortingWidget::changeGate()
{
    int id = comboPopulation->currentData().toInt();
    int detectorX = 0;
    int detectorY = 0;
    if (id == 0) {
        m_currGate = Gate();
    } else {
        m_currGate = GatesDAO().fetchGate(id);
        detectorX = DetectorSettingsDAO().getSettingDetectorId(m_currGate.xAxisSettingId());
        detectorY = DetectorSettingsDAO().getSettingDetectorId(m_currGate.yAxisSettingId());
    }
    emit gateChanged(m_currGate, detectorX, detectorY);
}

void SortingWidget::startSorting()
{
    if (btnRunSorting->text() == tr("Start Sort")) {
        btnRunSorting->setText(tr("Stop Sort"));
        CytometerController::instance()->startSorting();
    } else {
        btnRunSorting->setText(tr("Start Sort"));
        CytometerController::instance()->stopSorting();
    }
}



