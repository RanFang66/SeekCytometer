#ifndef DATAACQUISITIONWIDGET_H
#define DATAACQUISITIONWIDGET_H

#include <QDockWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

class DataAcquisitionWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DataAcquisitionWidget(const QString &tilte, QWidget *parent = nullptr);

signals:


private slots:
    void startAcquisition();
    void startRecordData();
    void restartAcquisition();

private:
    void initDockWidget();


    QProgressBar *progressTube;
    QLabel *lblThresholdRate;
    QLabel *lblStopGateEvents;
    QLabel *lblElapsedTime;
    QPushButton *btnAcquisition;
    QPushButton *btnRecord;
    QPushButton *btnRestart;
    QComboBox *cBoxStoppingGate;
    QComboBox *cBoxRecordEvents;
    QSpinBox *spinBoxStoppingTime;
    QComboBox *cBoxStorageGate;
    QComboBox *cBoxDisplayEvents;
    QComboBox *cBoxFlowRate;
    QLabel      *lblProcessEvenets;
    QLabel      *lblThresholdCount;
};

#endif // DATAACQUISITIONWIDGET_H
