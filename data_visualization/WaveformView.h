#ifndef WAVEFORMVIEW_H
#define WAVEFORMVIEW_H

#include <QtCharts>
#include <QChartView>
#include <QObject>
#include "Waveform.h"


enum class SAMPLE_CHANNEL : unsigned char {
    CH0 = 0,
    CH1,
    CH2,
    CH3,
    CH4,
    CH5,
    CH6,
    CH7,
};

constexpr SAMPLE_CHANNEL ALL_CHANNELS[] = {
    SAMPLE_CHANNEL::CH0,
    SAMPLE_CHANNEL::CH1,
    SAMPLE_CHANNEL::CH2,
    SAMPLE_CHANNEL::CH3,
    SAMPLE_CHANNEL::CH4,
    SAMPLE_CHANNEL::CH5,
    SAMPLE_CHANNEL::CH6,
    SAMPLE_CHANNEL::CH7,
};


constexpr int CHANNEL_NUM = sizeof(ALL_CHANNELS);
constexpr int CHANNEL_START = static_cast<int>(SAMPLE_CHANNEL::CH0);


static constexpr int channelInt(SAMPLE_CHANNEL channel) {
    return static_cast<int>(channel);
}

static bool isValidChannelIndex(int value) {
    return (value >= CHANNEL_START && value < CHANNEL_NUM) ? true : false;
}

static std::optional<SAMPLE_CHANNEL> toWaveformChannel(int value) {
    return (value >= CHANNEL_START && value < CHANNEL_NUM) ? std::optional<SAMPLE_CHANNEL>(static_cast<SAMPLE_CHANNEL>(value)) : std::nullopt;
}


class WaveformView : public QChartView
{
    Q_OBJECT
public:
    explicit WaveformView(QWidget *parent = nullptr);
    ~WaveformView();
    enum DragFunc {
        DRAG_FUNC_MOVE,
        DRAG_FUNC_SELC,
        DRAG_FUNC_NONE,
    };

    static const QColor chColor[16];

    void enableChannel(SAMPLE_CHANNEL ch);
    void disableChannel(SAMPLE_CHANNEL ch);
    void clearChannel(SAMPLE_CHANNEL ch);
    void resetRange();
    void changeDragFunc(DragFunc func);
    void addSeriesData(const QList<int> &data);

    void snapAPicture(QString &imgPath);
    void saveWaveformData(QString &filePath);

    void setXLimit(qreal min, qreal max);
    void setYLimit(qreal min, qreal max);
    void getXLimit(qreal &min, qreal &max) const;
    void getYLimit(qreal &min, qreal &max) const;
    qreal getXRangeLimit() const;
    void setXRangeLimit(qreal newXRangeLimit);
    qreal getYRangeLimit() const;
    void setYRangeLimit(qreal newYRangeLimit);

    void enableThresholdLIne(bool en);
    void setThresholdLineValue(int val);

    void setDisplayMode(bool showVoltage);
    bool isShowVoltage() const { return m_showVoltage; }

    int thresholdLineVal() const { return threshVal; }
public slots:
    void startGraph();
    void pauseGraph();

protected:
    enum DragMode {
        None,
        DragX,
        DragY,
        DragPlot
    };


    bool viewportEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent * event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    bool m_isTouching;
    bool m_pauseWave;
    bool m_showVoltage;


    void updateThresholdLine();
    Waveform            *waveform;
    QValueAxis          *axisX;
    QValueAxis          *axisY;
    QList<QLineSeries*>  waveSeries;
    QVector<QPointF>    m_waveBuffer[CHANNEL_NUM];
    int                 m_bufferIndex[CHANNEL_NUM] = {0};
    int                 m_maxWaveLength;

    QRubberBand         *rubberBand;
    QPoint              startPos;
    DragMode            dragMode;
    DragFunc            dragFunc;
    qreal               xMinLimit;
    qreal               xMaxLimit;
    qreal               yMinLimit;
    qreal               yMaxLimit;
    qreal               xRangeLimit;
    qreal               yRangeLimit;

    QLabel              *coordLabel;
    QLineSeries         *thresholdLine;

    QLabel              *threshValLabel;
    int               threshVal;

    void zoomAxisX(bool zoomIn);
    void setAxisXRange(qreal min, qreal max);
    void zoomAxisY(bool zoomIn);
    void setAxisYRange(qreal min, qreal max);

};

#endif // WAVEFORMVIEW_H
