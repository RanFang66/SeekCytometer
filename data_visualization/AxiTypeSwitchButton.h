#ifndef AXITYPESWITCHBUTTON_H
#define AXITYPESWITCHBUTTON_H

#include <QGraphicsObject>
#include <QObject>
#include "PlotBase.h"

class AxiTypeSwitchButton : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit AxiTypeSwitchButton(PlotBase *plot);

    QRectF boundingRect() const override;

    void paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *w) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    PlotBase *m_plot;
};

#endif // AXITYPESWITCHBUTTON_H
