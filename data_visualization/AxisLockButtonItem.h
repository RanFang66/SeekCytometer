#ifndef AXISLOCKBUTTONITEM_H
#define AXISLOCKBUTTONITEM_H

#include <QGraphicsObject>
#include <QObject>

#include "PlotBase.h"

class AxisLockButtonItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit AxisLockButtonItem(PlotBase *plot);

    QRectF boundingRect() const override;

    void paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *w) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    PlotBase *m_plot;

};

#endif // AXISLOCKBUTTONITEM_H
