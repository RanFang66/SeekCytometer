#ifndef POLYGONGATEITEM_H
#define POLYGONGATEITEM_H

#include "GateItem.h"

class PolygonGateItem : public GateItem
{
    Q_OBJECT
public:
    PolygonGateItem(const QPointF &startPoint, PlotBase *parent = nullptr);

    void updatePolygon(const QPointF &point);
    void updateGatePreview(const QPointF &point) override;
    void finishDrawing(const QPointF &point) override;
    GateType gateType() const override { return GateType::PolygonGate; }
    void updateGateData() override;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPolygonF       m_polygon;
};

#endif // POLYGONGATEITEM_H
