#include "MainWindow.h"

#include "DatabaseManager.h"
#include "ExperimentsBrowser.h"
#include "CytometerSettingsWidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    DatabaseManager::getInstance().connectToDatabase();

    ExperimentsBrowser *experimentsBrowser = new ExperimentsBrowser("Experiments Browser", this);
    addDockWidget(Qt::LeftDockWidgetArea, experimentsBrowser);
    CytometerSettingsWidget *cytometerSettingsWidget = new CytometerSettingsWidget("Cytometer Settings", this);
    addDockWidget(Qt::RightDockWidgetArea, cytometerSettingsWidget);
}

MainWindow::~MainWindow() {}





