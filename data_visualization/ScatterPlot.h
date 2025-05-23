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

private:
    ChartBuffer<QPointF>     m_data;
    static constexpr int    DEFAULT_DATA_LENGTH = 10000;
};


#endif // SCATTERPLOT_H
