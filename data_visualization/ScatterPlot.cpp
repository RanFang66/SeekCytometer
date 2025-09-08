#include "ScatterPlot.h"
#include <QMarginsF>




ScatterPlot::ScatterPlot(const Plot &plot, QGraphicsItem *parent)
    : PlotBase(plot, parent), m_data(DEFAULT_DATA_LENGTH)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | ItemClipsToShape);

    m_xAxis->setRange(5000, 80000);
    m_yAxis->setRange(5000, 80000);

    m_xAxis->setTicks(5);
    m_yAxis->setTicks(5);

    m_xAxis->setAxisName(plot.axisXName());
    m_yAxis->setAxisName(plot.axisYName());
}


void ScatterPlot::updateData(const QVector<QPointF> &data)
{
    if (data.isEmpty()) return;
    m_data.writeMultiple(data);
    m_xMin = m_xAxis->minValue();
    m_xMax = m_xAxis->maxValue();
    m_yMin = m_yAxis->minValue();
    m_yMax = m_yAxis->maxValue();
    QRectF plotRange(QPointF(m_xMin, m_yMax), QPointF(m_xMax, m_yMin));
    for (const QPointF &p : data) {
        if (!plotRange.contains(p)) {
            m_xMin = m_xMin > p.x() ? p.x() : m_xMin;
            m_xMax = m_xMax < p.x() ? p.x() : m_xMax;
            m_yMin = m_yMin > p.y() ? p.y() : m_yMin;
            m_yMax = m_yMax < p.y() ? p.y() : m_yMax;
        }
    }
    if (m_xMin != m_xAxis->minValue() || m_xMax != m_xAxis->maxValue()) {
        m_xAxis->setRange(m_xMin * 0.9, m_xMax * 1.1);
    }

    if (m_yMin != m_yAxis->minValue() || m_yMax != m_yAxis->maxValue()) {
        m_yAxis->setRange(m_yMin * 0.9, m_yMax * 1.1);
    }


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

void ScatterPlot::resetPlot()
{
    m_data.clear();
}

