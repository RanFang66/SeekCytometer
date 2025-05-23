#include "EllipseGateItem.h"
#include <QPainter>

EllipseGateItem::EllipseGateItem(const QPointF &startPosInPlot, PlotBase *parent)
    : GateItem(startPosInPlot, parent)
{
    m_ellipse = QRectF(0, 0, 0, 0);
}

void EllipseGateItem::updateGatePreview(const QPointF &point)
{
    prepareGeometryChange();
    m_previewPos = mapFromScene(point);
    m_ellipse.setWidth(m_previewPos.x());
    m_ellipse.setHeight(m_previewPos.y());
    update();
}

void EllipseGateItem::finishDrawing(const QPointF &point)
{
    updateGateData();
    prepareGeometryChange();
    m_ellipse = QRectF(QPointF(0, 0), mapFromScene(point));
    m_drawingFinished = true;
    update();
}

void EllipseGateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    if (m_drawingFinished) {
        painter->setPen(QPen(Qt::red, 2));
        painter->drawEllipse(m_ellipse);
    } else {
        painter->setPen(QPen(Qt::blue, 2, Qt::DashDotLine));
        painter->drawEllipse(m_ellipse);
    }

    painter->restore();
}

QRectF EllipseGateItem::boundingRect() const
{
    return m_ellipse;
}

void EllipseGateItem::updateGateData()
{
    QPointF p1 = mapToParent(m_ellipse.topLeft());
    QPointF p2 = mapToParent(m_ellipse.bottomRight());
    m_gate.setPoints({m_parent->mapPlotAreaToPoint(p1), m_parent->mapPlotAreaToPoint(p2)});
}

QPainterPath EllipseGateItem::shape() const
{
    QPainterPath path;
    path.addEllipse(m_ellipse);
    return path;
}

