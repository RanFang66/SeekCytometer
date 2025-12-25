#include "WaveformView.h"
#include <QMouseEvent>
#include <QLabel>

const QColor WaveformView::chColor[]={
    QColor (255,228,16),
    QColor (0,225,255),
    QColor (255,20,255),
    QColor (0,23,255),
    QColor (127,255,0),
    QColor (255,0,255),
    QColor (147,112,219),
    QColor (0,255,255),
    QColor (0, 0, 128),
    QColor (128, 0, 128),
    QColor (46, 139, 87),
    QColor (152, 251, 152),
    QColor (0, 255, 127),
    QColor (0, 0, 255),
    QColor (135, 206, 250),
    QColor (255, 192, 203),
};
#define AD_TO_MV (5000.0 / 131072.0)


WaveformView::WaveformView(QWidget *parent)
    : QChartView(parent),
    m_isTouching(false),
    m_pauseWave(true),
    dragMode(None),
    dragFunc(DRAG_FUNC_MOVE),
    m_showVoltage(false)
{
    xMinLimit = -10000;
    xMaxLimit = 100000;
    yMinLimit = -150000;
    yMaxLimit = 150000;
    xRangeLimit = 10;
    yRangeLimit = 10;
    setRubberBand(QChartView::NoRubberBand);
    setRenderHint(QPainter::Antialiasing, true);

    axisX = new QValueAxis;
    axisY = new QValueAxis;
    axisX->setTickCount(9);
    axisY->setTickCount(9);
    axisX->setLabelFormat("%d");
    axisY->setLabelFormat("%d");
    axisX->setRange(0, 10000);
    axisY->setRange(-1000, 100000);

    waveform = new Waveform;
    waveform->setTitle("Waveform");
    waveform->setAnimationOptions(QChart::SeriesAnimations);
    waveform->legend()->hide();
    waveform->setTheme(QChart::ChartThemeBlueIcy);
    waveform->addAxis(axisX, Qt::AlignBottom);
    waveform->addAxis(axisY, Qt::AlignLeft);

    setChart(waveform);
    m_maxWaveLength = 65536;
    for (int ch = CHANNEL_START; ch < CHANNEL_NUM; ch++) {
        auto *series = new QLineSeries();
        QPen pen;
        pen.setWidth(2);
        pen.setColor(chColor[ch]);
        series->setPen(pen);
        series->setUseOpenGL(true);
        waveSeries.append(series);
        waveform->addSeries(waveSeries[ch]);
        waveSeries[ch]->attachAxis(axisX);
        waveSeries[ch]->attachAxis(axisY);
        // waveSeries[ch]->setVisible(false);
    }
    threshVal = 0;
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    thresholdLine = new QLineSeries();


    thresholdLine->setPen(pen);
    thresholdLine->append(QPointF(0, threshVal));
    thresholdLine->append(QPointF(66000, threshVal));
    waveform->addSeries(thresholdLine);

    thresholdLine->attachAxis(axisX);
    thresholdLine->attachAxis(axisY);
    thresholdLine->setVisible(false);

    threshValLabel = new QLabel(QString("threshold: %1").arg(threshVal), this);
    threshValLabel->move(10, 10);

    threshValLabel->hide();
    threshValLabel->setStyleSheet("background-color: #333333; "
                                  "color: #FFFFFF; "
                                  "border: 1px solid white; "
                                  "border-radius: 5px; "
                                  "padding: 5px;");

    coordLabel = new QLabel(this);

    coordLabel->setStyleSheet("background-color: #333333; "
                              "color: #FFFFFF; "
                              "border: 1px solid white; "
                              "border-radius: 5px; "
                              "padding: 5px;");
    coordLabel->hide();
}

WaveformView::~WaveformView()
{
}

void WaveformView::enableChannel(SAMPLE_CHANNEL ch)
{
    waveSeries[channelInt(ch)]->setVisible(true);
}

void WaveformView::disableChannel(SAMPLE_CHANNEL ch)
{
    waveSeries[channelInt(ch)]->setVisible(false);
}

void WaveformView::clearChannel(SAMPLE_CHANNEL ch)
{
    waveSeries[channelInt(ch)]->clear();
}


void WaveformView::resetRange()
{
    axisX->setRange(0, 66000);
    axisY->setRange(-10000, 130000);
}

void WaveformView::changeDragFunc(DragFunc func)
{
    dragFunc = func;
}

void WaveformView::startGraph()
{
    m_pauseWave = false;
}

void WaveformView::pauseGraph()
{
    m_pauseWave = true;
}

void WaveformView::addSeriesData(const QList<int> &data)
{
    // int ch = channelInt(channel);
    // if (m_pauseWave == true) return;
    // int len = data.length();
    // int startIndex = m_bufferIndex[ch];

    // for (int i = 0; i < len; i++) {
    //     int bufferPos = (startIndex + i) % m_maxWaveLength;
    //     qreal val = m_showVoltage ? data.at(i) * AD_TO_MV : data.at(i);
    //     if (m_waveBuffer[ch].length() < m_maxWaveLength) {
    //         m_waveBuffer[ch].append(QPointF(bufferPos, val));
    //     } else {
    //         m_waveBuffer[ch][bufferPos].setY(val);
    //     }
    // }
    // m_bufferIndex[ch] = (startIndex + len) % m_maxWaveLength;
    // waveSeries[ch]->replace(m_waveBuffer[ch]);

    if (m_pauseWave) return;
    int len = data.length();
    for (int i = 0; i < len; i++) {
        int ch = data.at(i)  >> 24;
        if (ch < CHANNEL_START || ch >= CHANNEL_NUM) continue;

        int val_raw = ((data.at(i) << 14) >> 14);   // Expand signed bit
        qreal val = m_showVoltage ? val_raw * AD_TO_MV : val_raw;
        if (m_waveBuffer[ch].length() < m_maxWaveLength) {
            m_waveBuffer[ch].append(QPointF(m_bufferIndex[ch], val));
        } else {
            m_waveBuffer[ch][m_bufferIndex[ch]].setY(val);
        }
        m_bufferIndex[ch] =  (m_bufferIndex[ch] + 1) % m_maxWaveLength;
    }
    for (int ch = CHANNEL_START; ch < CHANNEL_NUM; ch++) {
        if (waveSeries[ch]->isVisible()) {
            waveSeries[ch]->replace(m_waveBuffer[ch]);
        }
    }
}

void WaveformView::snapAPicture(QString &imgPath)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap p = screen->grabWindow(winId());
    QImage img = p.toImage();
    img.save(imgPath);
}

void WaveformView::saveWaveformData(QString &filePath)
{
    QFile waveFile(filePath);
    float val = 0;
    if (waveFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&waveFile);
        for (int ch = CHANNEL_START; ch < CHANNEL_NUM; ch++) {
            if (waveSeries[ch]->isVisible()) {
                stream << QString("channel%1").arg(ch);
                stream << ",";
            }
        }
        stream << "\n";
        for (int i = 0; i < 65535; i++) {
            for (int ch = CHANNEL_START; ch < CHANNEL_NUM; ch++) {
                if (waveSeries[ch]->isVisible()) {
                    if (waveSeries[ch]->count() < i+1) {
                        val = 0;
                    } else {
                        val = waveSeries[ch]->at(i).y();
                    }
                    stream << QString::number(val, 'f', 2);
                    stream << " ,";
                }
            }
            stream << "\n";
        }
        waveFile.close();
    }
}

void WaveformView::setXLimit(qreal min, qreal max)
{
    xMinLimit = min;
    xMaxLimit = max;
}

void WaveformView::setYLimit(qreal min, qreal max)
{
    yMinLimit = min;
    yMaxLimit = max;
}

void WaveformView::getXLimit(qreal &min, qreal &max) const
{
    min = xMinLimit;
    max = xMaxLimit;
}

void WaveformView::getYLimit(qreal &min, qreal &max) const
{
    min = yMinLimit;
    max = yMaxLimit;
}

bool WaveformView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        // By default touch events are converted to mouse events. So
        // after this event we will get a mouse event also but we want
        // to handle touch events as gestures only. So we need this safeguard
        // to block mouse events that are actually generated from touch.
        m_isTouching = true;

        // Turn off animations when handling gestures they
        // will only slow us down.
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void WaveformView::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;

    if (event->button() == Qt::RightButton) {
        QPointF clickedPos = event->pos();
        QPointF chartValue = chart()->mapToValue(clickedPos);

        QString yValue;
        if (m_showVoltage) {
            yValue = QString::number(chartValue.y(), 'f', 1) + " mV";
        } else {
            yValue = QString::number(chartValue.y(), 'f', 0);
        }
        QString coordInfo = QString("Y: %1, X: %2").arg(yValue).arg(chartValue.x(), 0, 'f', 0);

        // QString coordInfo = QString("Y: %1,X: %2")
        //                         .arg(chartValue.y(), 0, 'f', 0)
        //                         .arg(chartValue.x(), 0, 'f', 0);

        coordLabel->setText(coordInfo);
        coordLabel->adjustSize();
        // Show coordLabel at the top right corner of the chart
        QRectF plotArea = chart()->plotArea();
        coordLabel->move(plotArea.right() - coordLabel->width(), plotArea.top());
        coordLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        coordLabel->show();
        return;
    }

    if (event->button() == Qt::LeftButton) {
        if (coordLabel->isVisible()) {
            coordLabel->hide();
        }

        startPos = event->pos();                    // Record mouse press start position
        QRectF plotArea = chart()->plotArea();      // Get polt area
        // Judge the mouse press start position in which area
        if (startPos.y() > plotArea.bottom() && startPos.x() >= plotArea.left() && startPos.x() <= plotArea.right()) {
            dragMode = DragX;
        } else if (startPos.x() < plotArea.left() && startPos.y() >= plotArea.top() && startPos.y() <= plotArea.bottom()) {
            dragMode = DragY;
        } else if (plotArea.contains(startPos)) {
            dragMode = DragPlot;
        } else {
            dragMode = None;
        }
        if (dragFunc == DRAG_FUNC_SELC) {
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            rubberBand->setGeometry(QRect(startPos, QSize()));
            rubberBand->show();
        }
    }
    QChartView::mousePressEvent(event);
}

void WaveformView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QPoint nowPos = event->pos();
    if (event->buttons() & Qt::LeftButton) {
        if (dragFunc == DRAG_FUNC_MOVE) {
            QPointF delta = chart()->mapToValue(startPos) - chart()->mapToValue(nowPos);
            if (dragMode == DragX) {
                setAxisXRange(axisX->min() + delta.x(), axisX->max() + delta.x());
            } else if (dragMode == DragY) {
                setAxisYRange(axisY->min() + delta.y(), axisY->max() + delta.y());
            } else if (dragMode == DragPlot) {
                setAxisXRange(axisX->min() + delta.x(), axisX->max() + delta.x());
                setAxisYRange(axisY->min() + delta.y(), axisY->max() + delta.y());
            }
            startPos = nowPos;
        } else if (dragFunc == DRAG_FUNC_SELC) {
            if (rubberBand) {
                rubberBand->setGeometry(QRect(startPos, nowPos).normalized());
            }
        }
    }

    QChartView::mouseMoveEvent(event);
}

void WaveformView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    // Because we disabled animations when touch event was detected
    // we must put them back on.
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    if (event->button() == Qt::LeftButton) {
        if (dragFunc == DRAG_FUNC_SELC && rubberBand) {
            QRectF selectedRect = rubberBand->geometry();
            QPointF topLeft = chart()->mapToValue(selectedRect.topLeft());
            QPointF bottomRight = chart()->mapToValue(selectedRect.bottomRight());
            if (dragMode == DragX) {
                setAxisXRange(topLeft.x(), bottomRight.x());
            } else if (dragMode == DragY) {
                setAxisYRange(bottomRight.y(), topLeft.y());
            } else if (dragMode == DragPlot && rubberBand) {
                setAxisXRange(topLeft.x(), bottomRight.x());
                setAxisYRange(bottomRight.y(), topLeft.y());
            }
            rubberBand->hide();
            delete rubberBand;
            rubberBand = nullptr;
        }
        dragMode = None;
    }
    QChartView::mouseReleaseEvent(event);
}

void WaveformView::wheelEvent(QWheelEvent *event)
{
    QPointF mousePos = event->position();
    QRectF plotArea = chart()->plotArea();

    bool zoomIn = event->angleDelta().y() > 0;

    if (mousePos.y() > plotArea.bottom() && mousePos.x() >= plotArea.left() && mousePos.x() <= plotArea.right()) {
        zoomAxisX(zoomIn);
    } else if (mousePos.x() < plotArea.left() && mousePos.y() >= plotArea.top() && mousePos.y() <= plotArea.bottom()) {
        zoomAxisY(zoomIn);
    } else if (plotArea.contains(mousePos)) {
        zoomAxisX(zoomIn);
        zoomAxisY(zoomIn);
    }

    event->accept();
}

void WaveformView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
        //![1]
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void WaveformView::updateThresholdLine()
{
    qreal yVal = m_showVoltage ? threshVal * AD_TO_MV : threshVal;
    thresholdLine->replace(QVector<QPointF>{QPointF(0, yVal), QPointF(66000, yVal)});
    // Keep the thesholdLine above the waveform line
    QString text = QString("threshold: %1%2")
                      .arg(yVal, 0, 'f', m_showVoltage ? 1 : 0)
                      .arg(m_showVoltage ? " mV" : "");
    threshValLabel->setText(text);
    threshValLabel->adjustSize();
}


qreal WaveformView::getXRangeLimit() const
{
    return xRangeLimit;
}

void WaveformView::setXRangeLimit(qreal newXRangeLimit)
{
    xRangeLimit = newXRangeLimit;
}

qreal WaveformView::getYRangeLimit() const
{
    return yRangeLimit;
}

void WaveformView::setYRangeLimit(qreal newYRangeLimit)
{
    yRangeLimit = newYRangeLimit;
}

void WaveformView::enableThresholdLIne(bool en)
{
    if (!en) {
        thresholdLine->hide();
        threshValLabel->hide();
    } else {
        thresholdLine->show();
        threshValLabel->show();
    }
}

void WaveformView::setThresholdLineValue(int val)
{
    if (threshVal != val) {
        threshVal = val;
        if (thresholdLine) {
            thresholdLine->clear();
            thresholdLine->append(QPointF(0, val));
            thresholdLine->append(QPointF(66000, val));
            thresholdLine->show();
            threshValLabel->setText(QString("threshold: %1").arg(val));
            threshValLabel->adjustSize();
            threshValLabel->show();
        }
    }
}

void WaveformView::setDisplayMode(bool showVoltage)
{
    if (m_showVoltage == showVoltage)
        return;
    m_showVoltage = showVoltage;

    // 保存当前Y轴范围并转换单位
    qreal currentMin = axisY->min();
    qreal currentMax = axisY->max();

    if (showVoltage) {
        // AD转mV
        currentMin *= AD_TO_MV;
        currentMax *= AD_TO_MV;
        axisY->setLabelFormat("%.1f mV");
    } else {
        // mV转AD
        currentMin /= AD_TO_MV;
        currentMax /= AD_TO_MV;
        axisY->setLabelFormat("%d");
    }

    // 应用新的范围限制
    qreal yMin = showVoltage ? yMinLimit * AD_TO_MV : yMinLimit;
    qreal yMax = showVoltage ? yMaxLimit * AD_TO_MV : yMaxLimit;
    currentMin = qBound(yMin, currentMin, yMax);
    currentMax = qBound(yMin, currentMax, yMax);

    // 确保最小间隔
    if (currentMax - currentMin < (showVoltage ? yRangeLimit * AD_TO_MV : yRangeLimit)) {
        currentMax = currentMin + (showVoltage ? yRangeLimit * AD_TO_MV : yRangeLimit);
    }

    axisY->setRange(currentMin, currentMax);
    updateThresholdLine();
}

void WaveformView::zoomAxisX(bool zoomIn)
{
    qreal range = axisX->max() - axisX->min();
    qreal center = (axisX->min() + axisX->max()) / 2.0;
    qreal scaleFactor = zoomIn ? 0.9 : 1.1;

    qreal newRange = range * scaleFactor;

    setAxisXRange(center - newRange / 2.0, center + newRange / 2.0);
}

void WaveformView::zoomAxisY(bool zoomIn)
{
    qreal range = axisY->max() - axisY->min();
    qreal center = (axisY->min() + axisY->max()) / 2.0;
    qreal scaleFactor = zoomIn ? 0.9 : 1.1;

    qreal newRange = range * scaleFactor;

    setAxisYRange(center - newRange / 2.0, center + newRange / 2.0);
}

void WaveformView::setAxisXRange(qreal min, qreal max)
{
    qreal minVal = qBound(xMinLimit, min, xMaxLimit);
    qreal maxVal = qBound(xMinLimit, max, xMaxLimit);
    if (maxVal - minVal < xRangeLimit) {
        maxVal = minVal + xRangeLimit;
    }
    axisX->setRange(minVal, maxVal);
}

void WaveformView::setAxisYRange(qreal min, qreal max)
{
    if (m_showVoltage) {
        // 输入是mV值，转换为AD处理限制
        qreal minAD = min / AD_TO_MV;
        qreal maxAD = max / AD_TO_MV;

        minAD = qBound(yMinLimit, minAD, yMaxLimit);
        maxAD = qBound(yMinLimit, maxAD, yMaxLimit);

        if (maxAD - minAD < yRangeLimit) {
            maxAD = minAD + yRangeLimit;
        }

        // 转换回mV显示
        axisY->setRange(minAD * AD_TO_MV, maxAD * AD_TO_MV);
    } else {
        qreal minVal = qBound(yMinLimit, min, yMaxLimit);
        qreal maxVal = qBound(yMinLimit, max, yMaxLimit);
        if (maxVal - minVal < yRangeLimit) {
            maxVal = minVal + yRangeLimit;
        }
        axisY->setRange(minVal, maxVal);
    }
}
