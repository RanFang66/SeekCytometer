#include "AxiTypeSwitchButton.h"
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>


AxiTypeSwitchButton::AxiTypeSwitchButton(PlotBase *plot)
    :QGraphicsObject(plot), m_plot(plot)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setCursor(Qt::PointingHandCursor);
}

QRectF AxiTypeSwitchButton::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

void AxiTypeSwitchButton::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *w)
{
    p->setRenderHint(QPainter::Antialiasing);


    QPixmap icon;
    if (m_plot->AxisType() == CustomAxis::Linear) {
        icon = QPixmap(":/resource/images/icons/linear_axis.png");
    } else {
        icon = QPixmap(":/resource/images/icons/log_axis.png");
    }



    p->drawPixmap(boundingRect().toRect(), icon);
}

void AxiTypeSwitchButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_plot->AxisType() == CustomAxis::Linear) {
        m_plot->changeAxisType(CustomAxis::Logarithmic);
    } else {
        m_plot->changeAxisType(CustomAxis::Linear);
    }
    update();
    event->accept();
}
