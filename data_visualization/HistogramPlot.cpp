#include "HistogramPlot.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

HistogramPlot::HistogramPlot(const Plot &plot, QGraphicsItem *parent)
    : PlotBase(plot, parent), m_data(DEFAULT_DATA_LENGTH)
{
    m_xAxis->setRange(0, 10000);
    m_yAxis->setRange(0, 100);

    m_xAxis->setTicks(5);
    m_yAxis->setTicks(5);

    m_xAxis->setAxisName(plot.axisXName());
    m_yAxis->setAxisName("Count");
}


void HistogramPlot::updateData(const QVector<int> &data)
{
    if (data.isEmpty()) return;
    m_data.writeMultiple(data);

    m_data.getMinMax(m_xMinVal, m_xMaxVal);

    qreal curMin = m_xAxis->minValue();
    qreal curMax = m_xAxis->maxValue();
    bool needUpdate = false;
    if (m_xMinVal < curMin) {
        curMin = m_xMinVal;
        needUpdate = true;
    }
    if (m_xMaxVal > curMax) {
        curMax = m_xMaxVal;
        needUpdate = true;
    }

    if (needUpdate) {
        m_xAxis->setRange(curMin, curMax);
    }

    update();
}


void HistogramPlot::paintPlot(QPainter *painter)
{
    if (!painter) return;

    const int binNum = qMax(1, static_cast<int>(m_plotArea.width()));
    if (m_bins.size() != binNum)
        m_bins.resize(binNum);

    m_bins.fill(0);
    m_maxValue = 0;

    const bool xLog = (m_xAxis->scaleType() == CustomAxis::Logarithmic);

    // ---------- 统计 ----------
    for (const int val : m_data.readAll()) {

        if (val <= 0 && xLog)
            continue;

        double ratio;
        if (xLog) {
            ratio = m_xAxis->mapValueToRatio(val);
        } else {
            ratio = (val - m_xAxis->minValue()) / m_xAxis->range();
        }

        int binIndex = static_cast<int>(ratio * binNum);
        binIndex = qBound(0, binIndex, binNum - 1);

        int &cnt = m_bins[binIndex];
        cnt++;
        m_maxValue = qMax(m_maxValue, cnt);
    }

    // ---------- 自动调整 Y 轴 ----------
    if (m_maxValue > m_yAxis->maxValue() * 0.9) {
        m_yAxis->setRange(
            m_yAxis->isLog() ? 1.0 : 0.0,
            m_maxValue * 1.1
            );
    } else if (m_maxValue * 1.3 < m_yAxis->maxValue()) {
        m_yAxis->setRange(m_yAxis->isLog() ? 1.0 : 0.0,
                          m_maxValue * 1.1);
    }

    painter->save();
    painter->setPen(Qt::blue);

    const bool yLog = (m_yAxis->scaleType() == CustomAxis::Logarithmic);

    // ---------- 绘制 ----------
    for (int i = 0; i < m_bins.size(); ++i) {
        int value = m_bins.at(i);
        if (value <= 0)
            continue;

        double yRatio;
        if (yLog) {
            yRatio = m_yAxis->mapValueToRatio(value);
        } else {
            yRatio = static_cast<double>(value) / m_yAxis->maxValue();
        }

        qreal x = m_plotArea.left() + i;
        qreal yTop = m_plotArea.bottom() - yRatio * m_plotArea.height();

        painter->drawLine(
            QPointF(x, m_plotArea.bottom()),
            QPointF(x, yTop)
            );
    }

    // ---------- 框选 ----------
    if (m_dragMode == DragRubberBand) {
        QRectF rect(m_rubberStartPos, m_rubberEndPos);
        rect = rect.normalized();

        QPen pen(Qt::DashLine);
        pen.setColor(Qt::darkGray);
        painter->setPen(pen);
        painter->setBrush(QColor(100, 100, 255, 40));

        painter->drawRect(rect);
    }

    painter->restore();
}

void HistogramPlot::resetPlot()
{
    m_data.clear();
}

void HistogramPlot::autoAdjustAxisRange()
{
    m_data.getMinMax(m_xMinVal, m_xMaxVal);
    m_xAxis->setRange(m_xMinVal, m_xMaxVal);
    update();
}

void HistogramPlot::changeAxisType(CustomAxis::ScaleType type)
{
    m_xAxis->setScaleType(type);
    update();
}

void HistogramPlot::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (!m_axisUnlocked) {
        event->ignore();
        return;
    }

    QPointF pos = event->pos();

    const double zoomFactor = (event->delta() > 0) ? 0.9 : 1.1;
    QPointF dataPos = mapPlotAreaToPoint(pos);

    if (m_axisXArea.contains(pos)) {
        zoomAxis(m_xAxis, dataPos.x(), zoomFactor);
    } else if (m_axisYArea.contains(pos)) {
        zoomAxis(m_yAxis, dataPos.y(), zoomFactor);
    } else {
        QGraphicsObject::wheelEvent(event);
        return;
    }

    update();
    event->accept();
}

void HistogramPlot::zoomAxis(CustomAxis *axis, double center, double factor)
{
    double min = axis->minValue();
    double max = axis->maxValue();

    double newMin = center - (center - min) * factor;
    double newMax = center + (max - center) * factor;

    if (newMax - newMin < 1e-6)
        return;

    axis->setRange(newMin, newMax);
}

void HistogramPlot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{ 
    if (event->button() != Qt::LeftButton) {
        QGraphicsObject::mousePressEvent(event);
        return;
    }
    if (!m_axisUnlocked) {
        if (!m_plotArea.contains(event->pos())) {
            QGraphicsObject::mousePressEvent(event);
            return;
        }
        m_cursorValue = mapPlotAreaToPoint(event->pos());
        m_showCursorValue = true;
        update();

        QTimer::singleShot(4000, this, [this]() {
            m_showCursorValue = false;
            update();
        });

        event->accept();
        return;
    }

    QPointF pos = event->pos();
    m_rubberStartPos = pos;
    m_rubberEndPos   = pos;
    if (m_plotArea.contains(pos)) {
        m_dragMode = DragRubberBand;
    } else if (m_axisXArea.contains(pos)) {
        m_dragMode = DragX;
    } else if (m_axisYArea.contains(pos)) {
        m_dragMode = DragY;
    } else {
        m_dragMode = NoDrag;
        QGraphicsObject::mousePressEvent(event);
        return;
    }

    event->accept();
}

void HistogramPlot::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_axisUnlocked || m_dragMode == NoDrag) {
        QGraphicsObject::mouseMoveEvent(event);
        return;
    }

    QPointF delta = event->pos() - m_rubberStartPos;
    double dValue = 0;
    switch (m_dragMode) {
    case DragX:
        dValue = delta.x() * m_xAxis->range() / m_plotArea.width();
        m_xAxis->setRange(
            m_xAxis->minValue() - dValue,
            m_xAxis->maxValue() - dValue
        );
        m_rubberStartPos = event->pos();
        break;
    case DragY:
        dValue = -delta.y() * m_yAxis->range() / m_plotArea.height();
        m_yAxis->setRange(
            m_yAxis->minValue() - dValue,
            m_yAxis->maxValue() - dValue
        );
        m_rubberStartPos = event->pos();
        break;
    case DragRubberBand:
        m_rubberEndPos = event->pos();
        break;
    default:
        QGraphicsObject::mouseMoveEvent(event);
        return;
    }

    update();
    event->accept();
}

void HistogramPlot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragMode != DragRubberBand || event->button() != Qt::LeftButton) {
        QGraphicsObject::mouseReleaseEvent(event);
        m_dragMode = NoDrag;
        return;
    }

    m_dragMode = NoDrag;

    QRectF rubberRect(m_rubberStartPos, m_rubberEndPos);
    rubberRect = rubberRect.normalized() & m_plotArea;

    if (rubberRect.width() < 10 || rubberRect.height() < 10) {
        update();
        return;
    }

    QPointF topRight = mapPlotAreaToPoint(rubberRect.topRight());
    QPointF bottomLeft = mapPlotAreaToPoint(rubberRect.bottomLeft());


    if (m_axisUnlocked) {
        m_xAxis->setRange(bottomLeft.x(), topRight.x());
        m_yAxis->setRange(bottomLeft.y(), topRight.y());
    }

    update();
    event->accept();
}

