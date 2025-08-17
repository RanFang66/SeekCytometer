#ifndef QUADRANTGATEITEM_H
#define QUADRANTGATEITEM_H

#include "GateItem.h"

class QuadrantGateItem : public GateItem
{
    Q_OBJECT
public:
    QuadrantGateItem(const QPointF &origin, PlotBase *parent = nullptr);
    QuadrantGateItem(const Gate &gate, PlotBase *parent = nullptr);
    void updateGatePreview(const QPointF &origin) override;
    void finishDrawing(const QPointF &origin) override;
    GateType gateType() const override { return GateType::QuadrantGate; }
    void updateGateData() override;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;


private:
    QPointF m_origin;
    QRectF  m_boundingRect;
};

#endif // QUADRANTGATEITEM_H
