#include "CytometerSettingsWidget.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QStatusBar>
#include <CytometerSettingsDAO.h>

CytometerSettingsWidget::CytometerSettingsWidget(const QString &title, QWidget *parent)
    : QDockWidget{title, parent},
    m_generalInfoWidget(new CytometerGeneralInfo(this)),
    m_detectorSettingsWidget(new DetectorSettingsWidget(this)),
    m_speedMeasureWidget(SpeedMeasureWidget::instance())
{
    initDockWidget();
}

CytometerSettingsWidget::~CytometerSettingsWidget()
{

}

const QList<DetectorSettings> &CytometerSettingsWidget::detectorSettings() const
{
    return m_detectorSettingsWidget->detectorSettings();
}

void CytometerSettingsWidget::onCytometerSettingsChanged(int cytometerSettingId)
{
    m_cytometerSettings = CytometerSettingsDAO().fetchCytometerSettings(cytometerSettingId);
    m_generalInfoWidget->onCytometerSettingsChanged(m_cytometerSettings);
    m_detectorSettingsWidget->onCytometerSettingChanged(cytometerSettingId);
    m_speedMeasureWidget->onCytometerSettingChanged();
}

void CytometerSettingsWidget::initDockWidget()
{
    QWidget     *mainWidget = new QWidget(this);
    QTabWidget  *tabWidget = new QTabWidget(mainWidget);
    QStatusBar  *statusBar = new QStatusBar(mainWidget);

    QVBoxLayout *layout = new QVBoxLayout(mainWidget);
    layout->addWidget(tabWidget);
    layout->addWidget(statusBar);

    tabWidget->addTab(m_generalInfoWidget, "General");
    tabWidget->addTab(m_detectorSettingsWidget, "Detectors");
    tabWidget->addTab(m_speedMeasureWidget, "Speed Measure");
    tabWidget->addTab(new QWidget(), "Lasers");
    tabWidget->addTab(new QWidget(), "Thresholds");
    tabWidget->addTab(new QWidget(), "Compensation");
    tabWidget->setCurrentIndex(1);
    statusBar->showMessage("Cytometer Connected");

    mainWidget->setLayout(layout);
    setWidget(mainWidget);
}
