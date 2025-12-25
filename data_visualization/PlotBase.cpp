#include "PlotBase.h"
#include <QPainter>
#include <QMarginsF>
#include <QFontMetrics>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include "GateItem.h"
#include "WorkSheetScene.h"


PlotBase::PlotBase(const Plot &plot, QGraphicsItem *parent)
: QGraphicsObject{parent}, m_plot{plot}
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | ItemClipsToShape);

    m_xAxis = new CustomAxis(this);
    m_yAxis = new CustomAxis(this);

    m_xAxis->setAlignment(Qt::AlignBottom);
    m_yAxis->setAlignment(Qt::AlignLeft);
    m_xAxis->setRange(0, 100);
    m_yAxis->setRange(0, 100);
    m_xAxis->setAxisName(m_plot.axisXName());
    m_yAxis->setAxisName(m_plot.axisYName());
    m_title = m_plot.plotName();
    m_titleFont = QFont("Arial", 12);

    m_boundingRect = QRectF(0, 0, 450, 450);
    updateLayout();
}

void PlotBase::setBoundingRect(const QRectF &rect)
{
    if (m_boundingRect != rect) {
        m_boundingRect = rect;
        updateLayout();
        update();
    }
}

void PlotBase::setTitle(const QString &title)
{
    if (m_title != title) {
        m_title = title;
        m_plot.setName(title);
        update();
    }
}

void PlotBase::setAxisXName(const QString &name)
{
    m_xAxis->setAxisName(name);
}

void PlotBase::setAxisYName(const QString &name)
{
    m_yAxis->setAxisName(name);
}



void PlotBase::updateLayout()
{
    QRectF validArea = m_boundingRect.marginsRemoved(QMarginsF(PLOT_MARGIN, PLOT_MARGIN, PLOT_MARGIN + 10, PLOT_MARGIN));

    QFontMetrics fm(m_titleFont);
    qreal titleHeight = fm.height() + TITLE_MARGIN;

    m_titleArea = QRectF(validArea.left(), validArea.top(), validArea.width(), titleHeight);

    m_plotArea = validArea;
    m_plotArea.setTop(m_titleArea.bottom());
    m_plotArea.setLeft(validArea.left() + m_yAxis->axisHeight());
    m_plotArea.setBottom(validArea.bottom() - m_xAxis->axisHeight());

    m_axisXArea = QRectF(m_plotArea.left(), m_plotArea.bottom(), m_plotArea.width(), m_xAxis->axisHeight());
    m_axisYArea = QRectF(validArea.left(), m_plotArea.top(), m_yAxis->axisHeight(), m_plotArea.height());

    qDebug() << "Title Area: " << m_titleArea;
    qDebug() << "Plot Area: " << m_plotArea;
    qDebug() << "Axis X Area: " << m_axisXArea;
    qDebug() << "Axis Y Area: " << m_axisYArea;
}

void PlotBase::paintTitle(QPainter *painter)
{
    if (!painter || m_title.isEmpty()) {
        return;
    }

    painter->save();
    painter->setFont(m_titleFont);
    painter->setPen(Qt::black);
    painter->drawText(m_titleArea, Qt::AlignCenter, m_title);
    painter->restore();
}

void PlotBase::paintAxis(QPainter *painter)
{
    if (!painter) {
        return;
    }

    painter->save();
    painter->setPen(Qt::black);
    painter->drawRect(m_plotArea);


    // Draw X Axis
    int xTickNum = m_xAxis->numTicks();
    if (xTickNum <= 0) {
        xTickNum = 5;
    }
    double xTickInterval = m_xAxis->range() / xTickNum;
    qreal xRatio = m_plotArea.width() / m_xAxis->range();
    for (int i = 0; i <= xTickNum; i++) {
        double val = m_xAxis->minValue() + i * xTickInterval;
        qreal posX = m_plotArea.left() + (val - m_xAxis->minValue()) * xRatio;
        painter->drawLine(QPointF(posX, m_axisXArea.top()), QPointF(posX, m_axisXArea.top() + 8));
        painter->drawText(QRectF(posX - 25, m_axisXArea.top() + 8, 50, 20), Qt::AlignCenter, QString::number((int)val));
    }

    // Draw X Axis Title Under the Axis
    painter->drawText(QRectF(m_axisXArea.left(), m_axisXArea.top() + 20, m_axisXArea.width(), m_axisXArea.height()-20), Qt::AlignCenter, m_xAxis->axisName());

    // Draw Y Axis
    int yTickNum = m_yAxis->numTicks();
    if (yTickNum <= 0) {
        yTickNum = 5;
    }
    double yTickInterval = m_yAxis->range() / yTickNum;
    qreal yRatio = m_plotArea.height() / m_yAxis->range();
    for (int i = 0; i <= yTickNum; i++) {
        double val = m_yAxis->minValue() + i * yTickInterval;
        qreal posY = m_plotArea.bottom() - (val - m_yAxis->minValue()) * yRatio;
        painter->drawLine(QPointF(m_axisYArea.right(), posY), QPointF(m_axisYArea.right() - 8, posY));
        painter->drawText(QRectF(m_axisYArea.right() - 60, posY - 15, 60, 20), Qt::AlignCenter, QString::number((int)val));
    }

    // Draw Y Axis Title At the Left of the Axis in Vertical Direction
    painter->save();
    painter->rotate(-90);
    painter->drawText(QRectF(-m_axisYArea.bottom(), m_axisYArea.left()-10, m_axisYArea.height(), 40), Qt::AlignCenter, m_yAxis->axisName());
    painter->restore();

    painter->restore();
}

void PlotBase::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    WorkSheetScene *workSheetScene = dynamic_cast<WorkSheetScene*>((scene()));
    if (!workSheetScene)
        return;

    QAction *deleteAction = menu.addAction(QString("Delete Plot %1").arg(title()));
    QObject::connect(deleteAction, &QAction::triggered, workSheetScene, [this, sc=workSheetScene]() {
            QMetaObject::invokeMethod(sc, "onDeletePlot", Qt::QueuedConnection,
                                      Q_ARG(PlotBase*, this));
    });

    for (QGraphicsItem *gate : childItems()) {
        GateItem *item = dynamic_cast<GateItem*>(gate);
        if (!item)
            continue;
        QAction *gateDeleteAction = menu.addAction(QString("Delete Gate %1").arg(item->getGateName()));
        QObject::connect(gateDeleteAction, &QAction::triggered, workSheetScene, [item, sc=workSheetScene]() {
            QMetaObject::invokeMethod(sc, "onDeleteGate", Qt::QueuedConnection,
                                      Q_ARG(GateItem*, item));
        });
    }

    QAction *selectedAction = menu.exec(event->screenPos());

    event->accept();
}

qreal PlotBase::mapValueToXAixs(qreal value) const
{
    return m_plotArea.left() + (value - m_xAxis->minValue()) * (m_plotArea.width() / m_xAxis->range());
}

qreal PlotBase::mapValueToYAixs(qreal value) const
{
    return m_plotArea.bottom() - (value - m_yAxis->minValue()) * (m_plotArea.height() / m_yAxis->range());
}

qreal PlotBase::mapXAxisToValue(qreal value) const
{
    return m_xAxis->minValue() + (value - m_plotArea.left()) * (m_xAxis->range() / m_plotArea.width());
}


qreal PlotBase::mapYAxisToValue(qreal value) const
{
    return m_yAxis->minValue() + (m_plotArea.bottom() - value) * (m_yAxis->range() / m_plotArea.height());
}

QPointF PlotBase::mapPointToPlotArea(const QPointF &point) const
{
    QPointF mappedPoint;
    mappedPoint.setX(m_plotArea.left() + (point.x() - m_xAxis->minValue()) * (m_plotArea.width() / m_xAxis->range()));
    mappedPoint.setY(m_plotArea.bottom() - (point.y() - m_yAxis->minValue()) * (m_plotArea.height() / m_yAxis->range()));
    return mappedPoint;
}



QPointF PlotBase::mapPointToPlotArea(qreal x, qreal y) const
{
    return mapPointToPlotArea(QPointF(x, y));
}

QPointF PlotBase::mapPlotAreaToPoint(const QPointF &point) const
{
    QPointF mappedPoint;
    mappedPoint.setX(m_xAxis->minValue() + (point.x() - m_plotArea.left()) * (m_xAxis->range() / m_plotArea.width()));
    mappedPoint.setY(m_yAxis->minValue() + (m_plotArea.bottom() - point.y()) * (m_yAxis->range() / m_plotArea.height()));
    return mappedPoint;
}

QRectF PlotBase::mapRectToPlotArea(const QRectF &rect) const
{
    QPointF topLeft = mapPointToPlotArea(rect.topLeft());
    QPointF bottomRight = mapPointToPlotArea(rect.bottomRight());
    return QRectF(topLeft, bottomRight);
}

QRectF PlotBase::mapRectToAxis(const QRectF &rect) const
{
    QPointF topLeft = mapPlotAreaToPoint(rect.topLeft());
    QPointF bottomRight = mapPlotAreaToPoint(rect.bottomRight());
    return QRectF(topLeft, bottomRight);
}

bool PlotBase::isInPlotArea(const QPointF &point) const
{
    return m_plotArea.contains(point);
}

QPointF PlotBase::limitPointInPlot(const QPointF &point) const
{
    QPointF limitedPoint = point;
    if (point.x() < m_plotArea.left()) {
        limitedPoint.setX(m_plotArea.left());
    } else if (point.x() > m_plotArea.right()) {
        limitedPoint.setX(m_plotArea.right());
    }

    if (point.y() < m_plotArea.top()) {
        limitedPoint.setY(m_plotArea.top());
    } else if (point.y() > m_plotArea.bottom()) {
        limitedPoint.setY(m_plotArea.bottom());
    }

    return limitedPoint;
}

QPointF PlotBase::limitScenePointInPlot(const QPointF &pointInScene) const
{
    QPointF pointInPlot = mapFromScene(pointInScene);
    QPointF limitedPoint =  limitPointInPlot(pointInPlot);
    return mapToScene(limitedPoint);
}

void PlotBase::updateAxisRange(int xMin, int xMax, int yMin, int yMax)
{
    int xMinVal = m_xAxis->minValue() > xMin ? xMin : m_xAxis->minValue();
    int xMaxVal = m_xAxis->maxValue() < xMax ? xMax : m_xAxis->maxValue();
    int yMinVal = m_yAxis->minValue() > yMin ? yMin : m_yAxis->minValue();
    int yMaxVal = m_yAxis->maxValue() < yMax ? yMax : m_yAxis->maxValue();

    m_xAxis->setRange(xMinVal, xMaxVal);
    m_yAxis->setRange(yMinVal, yMaxVal);
}

void PlotBase::updateAxisRanges(const Gate &gate)
{
    QList<QPoint> points = gate.points();

    int minX = m_xAxis->minValue();
    int maxX = m_xAxis->maxValue();
    int minY = m_yAxis->minValue();
    int maxY = m_yAxis->maxValue();


    for (const QPoint& point : points) {
        if (point.x() < minX) minX = point.x();
        if (point.x() > maxX) maxX = point.x();
        if (point.y() < minY) minY = point.y();
        if (point.y() > maxY) maxY = point.y();
    }
    m_xAxis->setRange(minX, maxX);
    m_yAxis->setRange(minY, maxY);
}


QRectF PlotBase::boundingRect() const
{
    return m_boundingRect;
}

void PlotBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!painter) {
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    // Draw the bounding rect
    painter->setPen(Qt::black);
    painter->setBrush(Qt::white);
    painter->drawRect(m_boundingRect);
    painter->restore();

    paintTitle(painter);
    paintAxis(painter);
    paintPlot(painter);
}

// void PlotBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
// {
//     if (event->button() == Qt::LeftButton) {
//         if (m_state == DrawingState::DrawingIdle && isDrawing()) {
//             m_state = DrawingState::DrawingStarted;
//             m_gateType = qobject_cast<WorkSheetScene*>(scene())->gateType();
//             Gate gate = Gate(m_plot.workSheetId(), "", m_gateType, m_plot.axisXId(), m_plot.xMeasurementType(), m_plot.axisYId(), m_plot.yMeasurementType());
//             m_gateItem = new RectangleGateItem(gate, this);
//             m_startPos = event->pos();
//             m_gateItem->setPos(m_startPos);
//             scene()->addItem(m_gateItem);
//             qDebug() << "Start Drawing Gate" << Gate::gateTypeToString(m_gateType) << "at pos" << m_startPos;
//         } else if (m_state == DrawingState::DrawingStarted) {
//             m_state = DrawingState::DrawingFinished;
//             qDebug() << "End Drawing Gate" << Gate::gateTypeToString(m_gateType) << "at pos" << event->pos();
//         }
//     }

//     QGraphicsObject::mousePressEvent(event);
// }

// void PlotBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
// {
//     if (m_state == DrawingState::DrawingStarted) {
//         if (m_gateType == GateType::RectangleGate) {
//             QPointF pos = event->pos();
//             qreal w = pos.x() - m_startPos.x();
//             qreal h = pos.y() - m_startPos.y();
//             RectangleGateItem *rectGate = qobject_cast<RectangleGateItem*>(m_gateItem);
//             if (rectGate) {
//                 rectGate->updateRectangle(w, h);
//             }
//         }
//     }
//     QGraphicsObject::mouseMoveEvent(event);
// }

// // void PlotBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
// // {
//     // if (event->button() == Qt::LeftButton && m_state == DrawingState::DrawingStarted) {
//     //     if (m_gateType == GateType::RectangleGate) {
//     //         RectangleGateItem *rectGate = qobject_cast<RectangleGateItem*>(m_gateItem);
//     //         if (rectGate) {
//     //             QRectF rect = rectGate->boundingRect();
//     //         }

//     //         m_state = DrawingState::DrawingFinished;
//     //         qDebug() << "End Drawing Gate" << Gate::gateTypeToString(m_gateType) << "at pos" << event->pos();
//     //     }
//     // }
//     // QGraphicsObject::mouseReleaseEvent(event);
// // }

// bool PlotBase::isDrawing() const
// {
//     WorkSheetScene *scene = qobject_cast<WorkSheetScene*>(this->scene());
//     if (scene) {
//         return scene->isDrawingGate();
//     }
//     return false;
// }

