#include "AxisAutoAdjustButton.h"
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

AxisAutoAdjustButton::AxisAutoAdjustButton(PlotBase *plot)
    :QGraphicsObject(plot), m_plot(plot)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setCursor(Qt::PointingHandCursor);
}

QRectF AxisAutoAdjustButton::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

void AxisAutoAdjustButton::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    p->setRenderHint(QPainter::Antialiasing);

    QPixmap icon = QPixmap(":/resource/images/icons/auto_adjust.png");

    p->drawPixmap(boundingRect().toRect(), icon);
}

void AxisAutoAdjustButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // m_plot->setAxisUnlocked(!m_plot->isAxisUnlocked());
    // update();
    m_plot->autoAdjustAxisRange();
    event->accept();
}
