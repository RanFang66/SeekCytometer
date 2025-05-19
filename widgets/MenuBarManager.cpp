#include "MenuBarManager.h"
#include <QMessageBox>
#include "UserManageDialog.h"

MenuBarManager::MenuBarManager(QMainWindow *parent) : QObject(parent), mainWindow(parent) {
    menuBar = new QMenuBar(parent);
    setupMenuBar();
}

QMenuBar* MenuBarManager::getMenuBar() const {
    return menuBar;
}

void MenuBarManager::setupMenuBar() {
    createFileMenu();
    createEditMenu();
    createViewMenu();
    createExperimentMenu();
    createDataMenu();
    createHelpMenu();
}




void MenuBarManager::createFileMenu() {
    QMenu *fileMenu = menuBar->addMenu("File");

    openAction = new QAction("Open", mainWindow);
    saveAction = new QAction("Save", mainWindow);
    exitAction = new QAction("Exit", mainWindow);

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(openAction, &QAction::triggered, this, &MenuBarManager::openFile);
    connect(saveAction, &QAction::triggered, this, &MenuBarManager::saveFile);
    connect(exitAction, &QAction::triggered, mainWindow, &QMainWindow::close);
}

void MenuBarManager::createEditMenu() {
    QMenu *editMenu = menuBar->addMenu("Edit");

    cutAction = new QAction("Cut", mainWindow);
    copyAction = new QAction("Copy", mainWindow);
    pasteAction = new QAction("Paste", mainWindow);

    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
}

void MenuBarManager::createViewMenu() {
    QMenu *viewMenu = menuBar->addMenu("View");

    showBrowser = new QAction("Broswer", mainWindow);
    showCytometer = new QAction("Cytometer", mainWindow);
    showWorkSheet = new QAction("WorkSheet", mainWindow);
    showAcquisition = new QAction("Acquisition Control", mainWindow);
    showSorting = new QAction("Sorting", mainWindow);
    resetLayout = new QAction("Reset Layout", mainWindow);

    viewMenu->addAction(showBrowser);
    viewMenu->addAction(showCytometer);
    viewMenu->addAction(showWorkSheet);
    viewMenu->addAction(showAcquisition);
    viewMenu->addAction(showSorting);
    viewMenu->addSeparator();
    viewMenu->addAction(resetLayout);
}

void MenuBarManager::createExperimentMenu() {
    QMenu *experimentMenu = menuBar->addMenu("Experiment");

    runExpAction = new QAction("Run Experiment", mainWindow);
    settingsAction = new QAction("Experiment Settings", mainWindow);

    experimentMenu->addAction(runExpAction);
    experimentMenu->addAction(settingsAction);

    connect(runExpAction, &QAction::triggered, this, &MenuBarManager::runExperiment);
}

void MenuBarManager::createCytometerMenu()
{
    QMenu *cytometerMenu = menuBar->addMenu("Cytometer");
    QAction *detailAction = new QAction("Cytometer Detail", mainWindow);
    QAction *viewConfigAction = new QAction("View Configurations...", mainWindow);

    cytometerMenu->addAction(detailAction);
    cytometerMenu->addAction(viewConfigAction);
}

void MenuBarManager::createWorkSheetMenu()
{
    QMenu *workSheetMenu = menuBar->addMenu("WorkSheet");
    QAction *newWorkSheetAction = new QAction("New WorkSheet", mainWindow);
    QAction *openWorkSheetAction = new QAction("Open WorkSheet...", mainWindow);

    workSheetMenu->addAction(newWorkSheetAction);
    workSheetMenu->addAction(openWorkSheetAction);
}

void MenuBarManager::createHelpMenu()
{
    QMenu *helpMenu = menuBar->addMenu("Help");
    QAction *aboutAction = new QAction("About", mainWindow);
    QAction *helpAction = new QAction("Help", mainWindow);

    helpMenu->addAction(aboutAction);
    helpMenu->addAction(helpAction);
}

void MenuBarManager::createDataMenu()
{
    QMenu *dataMenu = menuBar->addMenu("Data");
    QAction *importAction = new QAction("Import Data", mainWindow);
    QAction *exportAction = new QAction("Export Data", mainWindow);
    userManage = new QAction("User Management", mainWindow);
    dataMenu->addAction(importAction);
    dataMenu->addAction(exportAction);
    dataMenu->addSeparator();
    dataMenu->addAction(userManage);
    connect(userManage, &QAction::triggered, this, &MenuBarManager::openUserManageDialog);
}



void MenuBarManager::openFile() {
    QMessageBox::information(mainWindow, "Open", "Open file clicked");
}

void MenuBarManager::saveFile() {
    QMessageBox::information(mainWindow, "Save", "Save file clicked");
}

void MenuBarManager::runExperiment() {
    QMessageBox::information(mainWindow, "Experiment", "Running experiment...");
}

void MenuBarManager::openUserManageDialog()
{
    UserManageDialog dialog(mainWindow);
    dialog.exec();
}
