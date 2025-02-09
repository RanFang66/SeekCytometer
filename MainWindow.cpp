#include "MainWindow.h"

#include "ExperimentsBrowser.h"
#include "CytometerSettingsWidget.h"
#include "User.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initStatusBar();
    initMenuBar();
    setupToolBar();
    initDockWidgets();

    setWindowTitle(QString("SeekCytometer - %1").arg(User::loginUser().name()));
    setWindowState(Qt::WindowMaximized);
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
    ExperimentsBrowser *experimentsBrowser = new ExperimentsBrowser("Experiments Browser", this);
    addDockWidget(Qt::LeftDockWidgetArea, experimentsBrowser);
    CytometerSettingsWidget *cytometerSettingsWidget = new CytometerSettingsWidget("Cytometer Settings", this);
    addDockWidget(Qt::RightDockWidgetArea, cytometerSettingsWidget);
}





