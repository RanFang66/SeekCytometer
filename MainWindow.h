#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include "CustomStatusBar.h"
#include "MenuBarManager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    CustomStatusBar         *statusBar;
    MenuBarManager          *menuBarManager;
    QToolBar                *toolBar;


    void initStatusBar();
    void initMenuBar();
    void setupToolBar();
    void initDockWidgets();

};
#endif // MAINWINDOW_H
