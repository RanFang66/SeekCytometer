#ifndef ELLIPSEGATEITEM_H
#define ELLIPSEGATEITEM_H

#include "GateItem.h"

class EllipseGateItem : public GateItem
{
    Q_OBJECT
public:
    EllipseGateItem(const QPointF &startPosInPlot, PlotBase *parent = nullptr);
    EllipseGateItem(const Gate &gate, PlotBase *parent = nullptr);
    void updateGatePreview(const QPointF &point) override;
    void finishDrawing(const QPointF &point) override;
    QRectF boundingRect() const override;
    GateType gateType() const override { return GateType::EllipseGate; }
    void updateGateData() override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

private:
    QRectF m_ellipse;
};

#endif // ELLIPSEGATEITEM_H
