#include "PolygonGateItem.h"
#include <QPainter>

PolygonGateItem::PolygonGateItem(const QPointF &startPoint, PlotBase *parent)
    : GateItem(startPoint, parent)
{
    m_polygon << QPointF(0, 0);
}

void PolygonGateItem::updatePolygon(const QPointF &point)
{
    prepareGeometryChange();
    m_previewPos = mapFromScene(point);
    m_polygon << mapFromScene(point);
    update();
}

void PolygonGateItem::updateGatePreview(const QPointF &point)
{
    prepareGeometryChange();
    m_previewPos = mapFromScene(point);
    update();
}

void PolygonGateItem::finishDrawing(const QPointF &point)
{
    updateGateData();
    prepareGeometryChange();
    m_polygon << mapFromScene(point);
    m_drawingFinished = true;
    update();
}

void PolygonGateItem::updateGateData()
{
    QList<QPointF> points;
    for (const QPointF &point : m_polygon) {
        QPointF p = mapToParent(point);
        points.append(m_parent->mapPlotAreaToPoint(p));
    }
    m_gate.setPoints(points);
}



QRectF PolygonGateItem::boundingRect() const
{
    QRectF rect;
    if (m_drawingFinished) {
        rect = m_polygon.boundingRect();
    } else {
        QPolygonF polygonTmp = m_polygon;
        polygonTmp << m_previewPos;
        rect = polygonTmp.boundingRect();
    }

    qreal margin = 1;
    rect.adjust(-margin, -margin, margin, margin);
    return rect;
}

void PolygonGateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    if (m_drawingFinished) {
        painter->setPen(QPen(Qt::red, 2));
        painter->drawPolygon(m_polygon);
    } else {
        painter->setPen(QPen(Qt::blue, 2, Qt::DashDotLine));
        painter->drawPolyline(m_polygon);
        painter->drawLine(m_polygon.last(), m_previewPos);
    }
    painter->restore();
}
