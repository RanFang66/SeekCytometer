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
#include "gate.h"
class SortingWidget : public QDockWidget
{
    Q_OBJECT
public:
    static SortingWidget *instance()
    {
        static SortingWidget instance("Sort Control");
        return &instance;
    }
    void updatePopulation(int workSheetId);

    const Gate getCurrentPopulation() const;

signals:
    void driveParametersChanged(int type, int delay, int width, int coolingTime);
    void gateChanged(const Gate &gate, int detectorX, int detectorY);

private slots:
    void changeDriveParameters();
    void changeGate();
    void startSorting();


private:
    explicit SortingWidget(const QString &tilte = QString("Sorting"), QWidget *parent = nullptr);
    SortingWidget &operator=(const SortingWidget &) = delete;
    SortingWidget(const SortingWidget &) = delete;


    void initSortingWidget();

    int m_driveType;
    int m_driveDelay;
    int m_driveWidth;
    int m_coolingTime;

    Gate m_currGate;


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

    QLabel          *sortNum;
    QLabel          *abortNum;
    QLabel          *sortRate;
    QLabel          *abortRate;
    QLabel          *sortEfficiency;
    QProgressBar    *progressSort;
};

#endif // SORTINGWIDGET_H
