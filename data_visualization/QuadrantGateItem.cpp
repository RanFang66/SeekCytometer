#include "QuadrantGateItem.h"
#include <QPainter>

QuadrantGateItem::QuadrantGateItem(const QPointF &origin, PlotBase *parent)
    : GateItem(origin, parent), m_origin(mapFromParent(origin))
{
    setPos(parent->plotArea().topLeft());
    m_boundingRect = QRectF(0, 0, parent->plotArea().width(), parent->plotArea().height());
}

void QuadrantGateItem::updateGatePreview(const QPointF &origin)
{
    m_origin = mapFromScene(origin);
    prepareGeometryChange();
    update();
}

void QuadrantGateItem::finishDrawing(const QPointF &origin)
{
    updateGateData();
    m_origin = mapFromScene(origin);
    m_drawingFinished = true;
    prepareGeometryChange();
    update();
}

void QuadrantGateItem::updateGateData()
{
    QPointF p1 = mapToParent(m_origin);
    m_gate.setPoints({m_parent->mapPlotAreaToPoint(p1)});
}


QRectF QuadrantGateItem::boundingRect() const
{
    return m_boundingRect;
}

void QuadrantGateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    if (m_drawingFinished) {
        painter->setPen(QPen(Qt::red, 2));
        painter->drawLine(0, m_origin.y(), m_boundingRect.width(), m_origin.y());
        painter->drawLine(m_origin.x(), 0, m_origin.x(), m_boundingRect.height());
        painter->drawEllipse(m_origin, 3, 3);
    } else {
        painter->setPen(QPen(Qt::blue, 2, Qt::DashDotLine));
        painter->drawLine(0, m_origin.y(), m_boundingRect.width(), m_origin.y());
        painter->drawLine(m_origin.x(), 0, m_origin.x(), m_boundingRect.height());
        painter->drawEllipse(m_origin, 3, 3);
    }
    painter->restore();
}



