#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include <QGraphicsObject>
#include <QFont>
#include <QFontMetrics>
#include "ChartBuffer.h"
#include "PlotBase.h"


class HistogramPlot : public PlotBase
{
    Q_OBJECT
public:
    explicit HistogramPlot(const Plot &plot, QGraphicsItem *parent = nullptr);

public slots:
    void updateData(const QVector<int> &data);

protected:
    void            paintPlot(QPainter *painter) override;
    void            resetPlot() override;

    void autoAdjustAxisRange() override;
    void changeAxisType(CustomAxis::ScaleType type) override;

    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void zoomAxis(CustomAxis *axis, double center, double factor);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    static constexpr int DEFAULT_DATA_LENGTH = 60000;

    AxisDragMode m_dragMode = NoDrag;


    ChartBuffer<int>        m_data;
    QList<int>              m_bins;
    int                     m_maxValue;
    int                     m_xMinVal;
    int                     m_xMaxVal;
};

#endif // HISTOGRAMPLOT_H
