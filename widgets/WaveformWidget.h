#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QDockWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "WaveformView.h"
#include <QTimer>
#include <QSpinBox>

class WaveformWidget : public QDockWidget
{
    Q_OBJECT
public:
    static WaveformWidget *instance()
    {
        static WaveformWidget instance("Realtime Waveform");
        return &instance;
    }
    ~WaveformWidget();
signals:
    // void enableWaveform(bool en);
    // void waveformChannelsChanged(int);
    void waveformStateChanged(bool en, int channels, int interval);

public slots:
    void onReceivedWaveform(const QList<int> &data);


private slots:
    void onAddThresholdLine(bool checked);



private:
    explicit WaveformWidget(const QString &title, QWidget *parent = nullptr);
    WaveformWidget &operator=(const WaveformWidget &) = delete;
    WaveformWidget(const WaveformWidget &) = delete;


    void initDockWidget();

    QButtonGroup    *btnGrpWaveChEn;
    QComboBox       *comboSampleInterval;
    QPushButton     *btnRunWave;
    QPushButton     *btnAutoScale;
    QPushButton     *btnDragSelectOrMove;
    QPushButton     *btnGrabImage;
    QPushButton     *btnSaveData;
    QPushButton     *btnChangeAxis;
    QPushButton     *btnAddThresholdLine;
    QSpinBox        *spinThreshold;
    QPushButton     *btnSetDataPath;
    QLineEdit       *editSavePath;
    WaveformView    *waveView;


    bool            m_waveformEnabled;
    int             m_sampleChannels;
    int             m_sampleInterval;
    QString         m_waveformSavePath;
    // QTimer          *waveformGetTimer;
    // void            initWaveformWidget();

};

#endif // WAVEFORMWIDGET_H
