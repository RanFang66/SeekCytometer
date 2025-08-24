#ifndef SORTINGWIDGET_H
#define SORTINGWIDGET_H

#include <QDockWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QProgressBar>
#include "Gate.h"
#include <QTimer>


class SortingWidget : public QDockWidget
{
    Q_OBJECT
public:
    static SortingWidget *instance()
    {
        static SortingWidget instance("Sort Control");
        return &instance;
    }


    ~SortingWidget();

    const Gate getCurrentPopulation() const;

public slots:
    void updatePopulation();



signals:
    void driveParametersChanged(int type, int delay, int width, int coolingTime);
    void gateChanged(const Gate &gate);

private slots:
    void changeDriveParameters();
    void changeGate();
    void startSorting();
    void updateDisplay();


private:
    explicit SortingWidget(const QString &tilte = QString("Sorting"), QWidget *parent = nullptr);
    SortingWidget &operator=(const SortingWidget &) = delete;
    SortingWidget(const SortingWidget &) = delete;


    void initSortingWidget();
    void resetSortingStatus();


    int m_driveType;
    int m_driveDelay;
    int m_driveWidth;
    int m_coolingTime;

    Gate m_currGate;

    QTimer *updateTimer;

    int  m_sortTime;

    QPushButton *btnRunSorting;
    QPushButton *btnPauseSorting;
    QLineEdit   *editDriveWidth;
    QLineEdit   *editDriveStrength;
    QLineEdit   *editCoolingTime;
    QLineEdit   *editDriveDealy;
    QComboBox   *comboDriveMode;
    QComboBox   *comboPopulation;
    QComboBox   *comboSortMode;
    QCheckBox   *cBoxContinousMode;
    QLineEdit   *editTargetEvents;


    QLabel          *lblEventsNum;
    QLabel          *lblSortTime;
    QLabel          *lblSortNum;
    QLabel          *lblDiscardNum;
    QLabel          *lblProcessRate;
    QLabel          *lblSortRate;
    QLabel          *lblSortRatio;
    QLabel          *lblCellSpeed;
    QLabel          *lblDiscardRatio;


    // QProgressBar    *progressSort;
};

#endif // SORTINGWIDGET_H
