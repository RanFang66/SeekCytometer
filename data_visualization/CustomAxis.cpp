#include "CustomAxis.h"

#include <QFontMetrics>

CustomAxis::CustomAxis(QObject *parent, double minLimit, double maxLimit)
    : QObject{parent}, m_minValue(0), m_maxValue(10), m_numTicks(5), m_minLimit(minLimit), m_maxLimit(maxLimit),
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

void CustomAxis::setScaleType(ScaleType type)
{
    if (m_scaleType == type)
        return;

    m_scaleType = type;

    if (m_scaleType == Logarithmic) {
        // log 轴必须 > 0
        m_minLimit = 1e-1;
        m_maxLimit = 1e7;
        if (m_minValue < m_minLimit)
            m_minValue = m_minLimit;
        if (m_maxValue > m_maxLimit)
            m_maxValue = m_maxLimit;
        if (m_maxValue <= m_minValue)
            m_maxValue = m_minValue * 10.0;
    } else {
        m_minLimit = -100000;
        m_maxLimit = 1000000;
        if (m_minValue < m_minLimit)
            m_minValue = m_minLimit;
        if (m_maxValue > m_maxLimit)
            m_maxValue = m_maxLimit;
        if (m_maxValue <= m_minValue)
            m_maxValue = m_minValue + 1000;
    }
}


void CustomAxis::setRange(double min, double max)
{
    if (min < m_minLimit) {
        min = m_minLimit;
    }
    if (max > m_maxLimit) {
        max = m_maxLimit;
    }
    if (max <= min) {
        max = m_scaleType == Logarithmic ? min * 10 : min+10;
    }

    m_minValue = min;
    m_maxValue = max;
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

double CustomAxis::mapValueToRatio(double value) const
{
    if (m_scaleType == Linear) {
        return (value - m_minValue) / (m_maxValue - m_minValue);
    } else {
        double logMin = std::log10(m_minValue);
        double logMax = std::log10(m_maxValue);
        return (std::log10(value) - logMin) / (logMax - logMin);
    }
}

double CustomAxis::mapRatioToValue(double ratio) const
{
    ratio = qBound(0.0, ratio, 1.0);

    if (m_scaleType == Linear) {
        return m_minValue + ratio * (m_maxValue - m_minValue);
    } else {
        double logMin = std::log10(m_minValue);
        double logMax = std::log10(m_maxValue);
        return std::pow(10.0, logMin + ratio * (logMax - logMin));
    }
}

QList<AxisTick> CustomAxis::generateLogTicks() const
{
    QList<AxisTick> ticks;

    if (m_scaleType != Logarithmic)
        return ticks;

    if (m_minValue <= 0 || m_maxValue <= 0)
        return ticks;

    int minExp = std::floor(std::log10(m_minValue));
    int maxExp = std::ceil(std::log10(m_maxValue));

    for (int exp = minExp; exp <= maxExp; ++exp) {
        double base = std::pow(10.0, exp);

        // 主刻度：10^n
        if (base >= m_minValue && base <= m_maxValue) {
            ticks.append({base, true});
        }

        // 次刻度：2~9 * 10^n
        for (int i = 2; i <= 9; ++i) {
            double val = i * base;
            if (val >= m_minValue && val <= m_maxValue) {
                ticks.append({val, false});
            }
        }
    }

    return ticks;
}



