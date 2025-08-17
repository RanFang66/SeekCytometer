#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include <QGraphicsObject>
#include <QFont>
#include <QFontMetrics>
#include "ChartBuffer.h"
#include "PlotBase.h"


class HistogramPlot : public PlotBase
{
    Q_OBJECT
public:
    explicit HistogramPlot(const Plot &plot, QGraphicsItem *parent = nullptr);

public slots:
    void updateData(const QVector<double> &data);

protected:
    void            paintPlot(QPainter *painter) override;

private:
    static constexpr int DEFAULT_DATA_LENGTH = 60000;

    ChartBuffer<qreal>      m_data;
    QList<int>              m_bins;
    int                     m_maxValue;
    qreal                   m_xMinVal;
    qreal                   m_xMaxVal;
};

#endif // HISTOGRAMPLOT_H
