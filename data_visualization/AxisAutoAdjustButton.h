#ifndef AXISAUTOADJUSTBUTTON_H
#define AXISAUTOADJUSTBUTTON_H

#include <QGraphicsObject>
#include <QObject>
#include "PlotBase.h"

class AxisAutoAdjustButton : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit AxisAutoAdjustButton(PlotBase *plot);

    QRectF boundingRect() const override;

    void paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *w) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    PlotBase *m_plot;
};

#endif // AXISAUTOADJUSTBUTTON_H
