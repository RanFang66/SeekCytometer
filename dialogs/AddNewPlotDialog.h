#ifndef ADDNEWPLOTDIALOG_H
#define ADDNEWPLOTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include "Plot.h"


class AddNewPlotDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddNewPlotDialog(PlotType plotType, int worksheetId, QWidget *parent = nullptr);
    QString xAxisName() const;
    QString yAxisName() const;
    int xAxisId() const;
    int yAxisId() const;
    PlotType plotType() const;
    QString plotName() const;
    MeasurementType xMeasurementType() const;
    MeasurementType yMeasurementType() const;

    Plot getPlot() const;


private slots:
    void updateDefaultPlotName();

private:
    void initDialog();

    QComboBox *xAxisCombo;
    QComboBox *yAxisCombo;
    MeasurementType m_xMeasurementType;
    MeasurementType m_yMeasurementType;

    QString m_xAxisName;
    QString m_yAxisName;
    int     m_xAxisId;
    int     m_yAxisId;
    QString m_plotName;

    QLineEdit *plotNameEdit;

    PlotType m_plotType;
    int     m_worksheetId;
};

#endif // ADDNEWPLOTDIALOG_H
