#include "CytometerSettingsWidget.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QStatusBar>

CytometerSettingsWidget::CytometerSettingsWidget(const QString &title, QWidget *parent)
    : QDockWidget{title, parent}
{
    initDockWidget();
}

CytometerSettingsWidget::~CytometerSettingsWidget()
{

}

void CytometerSettingsWidget::initDockWidget()
{
    m_detectorSettingsModel = new DetectorSettingsModel();
    m_detectorSettingsWidget = new DetectorSettingsWidget(m_detectorSettingsModel);
    QWidget     *mainWidget = new QWidget(this);
    QTabWidget  *tabWidget = new QTabWidget(mainWidget);
    QStatusBar  *statusBar = new QStatusBar(mainWidget);

    QVBoxLayout *layout = new QVBoxLayout(mainWidget);
    layout->addWidget(tabWidget);
    layout->addWidget(statusBar);

    tabWidget->addTab(new QWidget(), "General");
    tabWidget->addTab(new QWidget(), "Lasers");
    tabWidget->addTab(m_detectorSettingsWidget, "Detectors");
    tabWidget->addTab(new QWidget(), "Thresholds");
    tabWidget->addTab(new QWidget(), "Compensation");

    statusBar->showMessage("Cytometer Connected");

    mainWidget->setLayout(layout);
    setWidget(mainWidget);
}
