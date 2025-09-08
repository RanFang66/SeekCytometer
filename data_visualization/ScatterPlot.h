#ifndef SCATTERPLOT_H
#define SCATTERPLOT_H

#include <QGraphicsObject>
#include <QPainter>
#include "PlotBase.h"
#include <QFont>
#include <QFontMetrics>
#include "ChartBuffer.h"

class ScatterPlot : public PlotBase
{
    Q_OBJECT
public:
    ScatterPlot(const Plot &plot, QGraphicsItem *parent = nullptr);

public slots:
    void updateData(const QVector<QPointF> &data);

protected:
    void            paintPlot(QPainter *painter) override;
    void            resetPlot() override;

private:
    ChartBuffer<QPointF>     m_data;
    static constexpr int    DEFAULT_DATA_LENGTH = 60000;

    qreal m_xMin, m_xMax, m_yMin, m_yMax;
};


#endif // SCATTERPLOT_H
