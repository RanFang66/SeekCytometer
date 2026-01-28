#include "AxisLockButtonItem.h"
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

AxisLockButtonItem::AxisLockButtonItem(PlotBase *plot)
    :QGraphicsObject(plot), m_plot(plot)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setCursor(Qt::PointingHandCursor);
}

QRectF AxisLockButtonItem::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

void AxisLockButtonItem::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    p->setRenderHint(QPainter::Antialiasing);

    QPixmap icon = m_plot->isAxisUnlocked()
        ? QPixmap(":/resource/images/icons/view_nofullscreen.png")
        : QPixmap(":/resource/images/icons/file_locked.png");


    p->drawPixmap(boundingRect().toRect(), icon);
}

void AxisLockButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_plot->setAxisUnlocked(!m_plot->isAxisUnlocked());
    update();
    event->accept();
}


