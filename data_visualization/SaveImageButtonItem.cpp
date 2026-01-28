#include "SaveImageButtonItem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

SaveImageButtonItem::SaveImageButtonItem(PlotBase *plot)
    :QGraphicsObject(plot), m_plot(plot)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setCursor(Qt::PointingHandCursor);
}

QRectF SaveImageButtonItem::boundingRect() const
{
    return QRectF(0, 0, 20 , 20);
}

void SaveImageButtonItem::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(Qt::white);
    p->setPen(Qt::black);
    p->drawRect(boundingRect());

    QPixmap icon = QPixmap(":/resource/images/icons/filesaveas.png");
    p->drawPixmap(boundingRect().toRect(), icon);

}

void SaveImageButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_plot->saveToImage();
    event->accept();
}



