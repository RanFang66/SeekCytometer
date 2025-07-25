#include "MainWindow.h"
#include <QMessageBox>

#include "ExperimentsBrowser.h"
#include "CytometerSettingsWidget.h"
#include "DataAcquisitionWidget.h"
#include "WorkSheetWidget.h"
#include "User.h"
#include "CytometerController.h"
#include "SortingWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initStatusBar();
    initMenuBar();
    setupToolBar();
    initDockWidgets();

    setWindowTitle(QString("SeekCytometer - %1").arg(User::loginUser().name()));
    setWindowState(Qt::WindowMaximized);

    CytometerController::instance()->start();
    connect(CytometerController::instance(), &CytometerController::connected, this, [this](){
        statusBar->updateConnectInfo(StatusIndicator::STATUS_RUNNING, tr("Connected to Server"));
        QMessageBox::information(this, tr("Connected"), tr("Connected with Cytometer"));
    });

    connect(CytometerController::instance(), &CytometerController::disconnected, this, [this](){
        statusBar->updateConnectInfo(StatusIndicator::STATUS_IDLE, tr("Disconnected"));
        QMessageBox::warning(this, tr("Disconnected"), tr("Disconnected with Cytometer"));
    });

    connect(CytometerController::instance(), &CytometerController::errorOccurred, this, [this](){
        statusBar->updateConnectInfo(StatusIndicator::STATUS_FAULT, tr("Fault"));
        QMessageBox::critical(this, tr("Fault!"), tr("There is critical fault"));
    });



    // QTimer::singleShot(1000, this, [this]() {
    //     CytometerController::instance()->connect();
    // });

}

MainWindow::~MainWindow() {}


void MainWindow::initStatusBar()
{
    statusBar = new CustomStatusBar;
    setStatusBar(statusBar);
}

void MainWindow::initMenuBar()
{
    menuBarManager = new MenuBarManager(this);
    setMenuBar(menuBarManager->getMenuBar());
}

void MainWindow::setupToolBar()
{
    toolBar = addToolBar("Main Toolbar");

    toolBar->addAction(menuBarManager->getSaveAction());
    toolBar->addSeparator();
    toolBar->addAction(menuBarManager->getShowBrowserAction());
    toolBar->addAction(menuBarManager->getShowCytometerAction());
    toolBar->addAction(menuBarManager->getShowWorkSheetAction());
    toolBar->addAction(menuBarManager->getShowAcquisitionAction());
    toolBar->addAction(menuBarManager->getShowSortingAction());
    toolBar->addSeparator();
    toolBar->addAction(menuBarManager->getResetLayoutAction());
}

void MainWindow::initDockWidgets()
{
    QWidget *p = takeCentralWidget();
    if (p) {
        delete p;
    }
    setDockNestingEnabled(true);


    ExperimentsBrowser *experimentsBrowser = new ExperimentsBrowser("Experiments Browser", this);
    addDockWidget(Qt::LeftDockWidgetArea, experimentsBrowser);


    CytometerSettingsWidget *cytometerSettingsWidget = new CytometerSettingsWidget("Cytometer", this);
    splitDockWidget(experimentsBrowser, cytometerSettingsWidget, Qt::Horizontal);

    // WorkSheetWidget *workSheetWidget = new WorkSheetWidget("WorkSheet", this);
    splitDockWidget(cytometerSettingsWidget, WorkSheetWidget::instance(), Qt::Horizontal);

    DataAcquisitionWidget *acquisitionWidget = new DataAcquisitionWidget("Acquisition Control", this);
    splitDockWidget(cytometerSettingsWidget, acquisitionWidget, Qt::Vertical);

    SortingWidget *sortingWidget = new SortingWidget("Sorting Control", this);
    splitDockWidget(acquisitionWidget, sortingWidget, Qt::Vertical);

    connect(experimentsBrowser, &ExperimentsBrowser::worksheetSelected, WorkSheetWidget::instance(), &WorkSheetWidget::addWorkSheetView);
    connect(experimentsBrowser, &ExperimentsBrowser::settingsSelected, cytometerSettingsWidget, &CytometerSettingsWidget::onCytometerSettingsChanged);


    // QList<QDockWidget *> docks = {experimentsBrowser, cytometerSettingsWidget};
    // QList<int> sizes = {3, 7};
    // resizeDocks(docks, sizes, Qt::Horizontal);
}





