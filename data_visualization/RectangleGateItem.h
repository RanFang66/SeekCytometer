#ifndef RECTANGLEGATEITEM_H
#define RECTANGLEGATEITEM_H

#include "GateItem.h"

class RectangleGateItem : public GateItem
{
    Q_OBJECT
public:
    RectangleGateItem(const QPointF &startPos, PlotBase *parent = nullptr);
    void updateRectangle(qreal w, qreal h);

    void updateGatePreview(const QPointF &point) override;
    void finishDrawing(const QPointF &point) override;
    QRectF boundingRect() const override;
    GateType gateType() const override { return GateType::RectangleGate; }
    void updateGateData() override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QRectF m_rectangle;
};

#endif // RECTANGLEGATEITEM_H
