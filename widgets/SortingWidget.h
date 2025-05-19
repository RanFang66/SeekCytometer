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

class SortingWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit SortingWidget(const QString &tilte = QString("Sorting"), QWidget *parent = nullptr);

private:
    void initSortingWidget();

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
