#include "StatusIndicator.h"

StatusIndicator::StatusIndicator(QWidget *parent)
    : QWidget{parent},
    m_status(0)
{
    update();
}

void StatusIndicator::setStatus(int newStatus)
{
    if (m_status != newStatus) {
        m_status = newStatus;
        update();
        emit statusChanged(newStatus);
    }
}

void StatusIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor color;
    switch (m_status) {
    case STATUS_IDLE:       color = Qt::gray;   break;
    case STATUS_RUNNING:    color = Qt::green;  break;
    case STATUS_FAULT:      color = Qt::red;    break;
    default:                color = Qt::gray;
    }

    painter.setBrush(QBrush(color));
    painter.setPen(Qt::NoPen);

    auto width = this->width();
    auto height = this->height();
    auto r = width > height ? height / 2: width / 2;

    painter.drawEllipse(QPoint(width / 2, height / 2), r, r);
}
