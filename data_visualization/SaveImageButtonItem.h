#ifndef SAVEIMAGEBUTTONITEM_H
#define SAVEIMAGEBUTTONITEM_H

#include <QGraphicsObject>
#include <QObject>
#include "PlotBase.h"

class SaveImageButtonItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit SaveImageButtonItem(PlotBase *plot);

    QRectF boundingRect() const override;

    void paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *w) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


private:
    PlotBase *m_plot;

};

#endif // SAVEIMAGEBUTTONITEM_H
