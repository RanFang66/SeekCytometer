#include "HistogramPlot.h"

#include <QPainter>

HistogramPlot::HistogramPlot(const Plot &plot, QGraphicsItem *parent)
    : PlotBase(plot, parent), m_data(ChartBuffer<qreal>(DEFAULT_DATA_LENGTH))
{
    m_xAxis->setRange(5000, 80000);
    m_yAxis->setRange(0, 100);

    m_xAxis->setTicks(5);
    m_yAxis->setTicks(5);

    m_xAxis->setAxisName(plot.axisXName());
    m_yAxis->setAxisName("Count");
}


void HistogramPlot::updateData(const QVector<double> &data)
{
    if (data.isEmpty()) return;
    m_data.writeMultiple(data);
    int binNums = m_plotArea.width();
    m_bins.fill(0, binNums);
    m_maxValue = 0;

    m_xMinVal = m_xAxis->minValue();
    m_xMaxVal = m_xAxis->maxValue();
    for (const double val : m_data.readAll()) {
        int binIndex = (val - m_xAxis->minValue()) * binNums / m_xAxis->range();
        if (binIndex < 0) binIndex = 0;
        if (binIndex >= m_bins.size()) binIndex = m_bins.size() - 1;
        m_bins[binIndex]++;
        if (m_bins[binIndex] > m_maxValue) {
            m_maxValue = m_bins[binIndex];
        }
        if (val < m_xMinVal) m_xMinVal = val;
        if (val > m_xMaxVal) m_xMaxVal = val;
    }


    if (m_maxValue > m_yAxis->maxValue() * 0.9) {
        m_yAxis->setRange(0, m_maxValue * 1.1);
    }

    if (m_xMinVal != m_xAxis->minValue() || m_xMaxVal != m_xAxis->maxValue()) {
        m_xAxis->setRange(m_xMinVal * 0.9, m_xMaxVal * 1.1);
    }

    update();
}


void HistogramPlot::paintPlot(QPainter *painter)
{
    if (!painter) return;
    painter->save();
    painter->setPen(Qt::blue);
    for (int i = 0; i < m_bins.size(); i++) {
        int value = m_bins.at(i);
        if (value <= 0)
            continue;
        qreal normalizeHeight = (static_cast<qreal>(value) / m_yAxis->maxValue()) * m_axisYArea.height();
        painter->drawLine(m_plotArea.left() + i, m_plotArea.bottom(), m_plotArea.left() + i, m_plotArea.bottom() - normalizeHeight);
    }
    painter->restore();
}

