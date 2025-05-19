#include "ScatterPlot.h"
#include <QMarginsF>




ScatterPlot::ScatterPlot(const Plot &plot, QGraphicsItem *parent)
    : PlotBase(plot, parent), m_data(DEFAULT_DATA_LENGTH)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | ItemClipsToShape);

    m_xAxis->setRange(0, 131072);
    m_yAxis->setRange(0, 131072);

    m_xAxis->setTicks(5);
    m_yAxis->setTicks(5);

    m_xAxis->setAxisName(plot.axisXName());
    m_yAxis->setAxisName(plot.axisYName());
}


void ScatterPlot::updateData(const QVector<QPointF> &data)
{
    if (data.isEmpty()) return;
    m_data.writeMultiple(data);
    update();
}


void ScatterPlot::paintPlot(QPainter *painter)
{
    if (!painter) return;

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::blue);
    for (const QPointF &point : m_data.readAll()) {
        painter->drawPoint(mapPointToPlotArea(point));
    }

    // painter->setPen(Qt::black);
    // painter->setBrush(Qt::blue);

    // QHash<QPoint, int> pointsDensity;

    // for (const QPointF &point : m_data.readAll()) {
    //     QPoint mappedPoint = mapPointToPlotArea(point).toPoint();
    //     pointsDensity[mappedPoint]++;
    // }

    // for (const QPoint &point : pointsDensity.keys()) {
    //     qreal density = pointsDensity[point];
    //     QColor color = QColor::fromHsvF(0.6, 1.0, density / 100);
    //     painter->setPen(color);
    //     painter->setBrush(color);
    //     painter->drawPoint(point);
    // }

    painter->restore();
}

