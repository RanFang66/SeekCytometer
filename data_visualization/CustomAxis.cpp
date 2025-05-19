#include "CustomAxis.h"

#include <QFontMetrics>

CustomAxis::CustomAxis(QObject *parent)
    : QObject{parent}, m_minValue(0), m_maxValue(10), m_numTicks(5),
      m_scaleType(Linear), m_title("Axis"), m_orientation(Qt::Horizontal),m_alignment(Qt::AlignBottom)
{
    m_titleFont = QFont("Helvetica", 14);
    m_tickLabelFont = QFont("Helvetica", 12);

    QFontMetrics fmTitle(m_titleFont);
    QFontMetrics fmLabel(m_tickLabelFont);
    m_axisTitleHeight = fmTitle.height() + AXIS_TITLE_MARGIN;
    m_axisTickHeight = fmLabel.height() + AXIS_TICK_MARGIN;
    m_axisHeight = m_axisTitleHeight + m_axisTickHeight;
}


void CustomAxis::setAlignment(Qt::Alignment alignment)
{
    switch (alignment) {
    case Qt::AlignTop:
    case Qt::AlignBottom:
        m_orientation = Qt::Horizontal;
        break;
    case Qt::AlignLeft:
    case Qt::AlignRight:
        m_orientation = Qt::Vertical;
        break;
    default:
        qWarning("CustomAxis::setAlignment: Invalid alignment");
        break;
    }
    m_alignment = alignment;
}

void CustomAxis::setTitleFont(const QFont &font)
{
    m_titleFont = font;
    QFontMetrics fm(m_titleFont);
    m_axisTitleHeight = fm.height() + AXIS_TITLE_MARGIN;
    m_axisHeight = m_axisTitleHeight + m_axisTickHeight;
}

void CustomAxis::setTickLabelFont(const QFont &font)
{
    m_tickLabelFont = font;
    QFontMetrics fm(m_tickLabelFont);
    m_axisTickHeight = fm.height() + AXIS_TICK_MARGIN;
    m_axisHeight = m_axisTitleHeight + m_axisTickHeight;
}



