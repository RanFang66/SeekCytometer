#include "WorkSheetScene.h"
#include <QGraphicsSceneMouseEvent>
#include "GateItemFactory.h"
#include "HistogramPlot.h"
#include "ScatterPlot.h"

WorkSheetScene::WorkSheetScene(QObject *parent)
    : QGraphicsScene{parent}, m_drawState{DrawingState::DrawingIdle}, m_gateType(GateType::UnknownGate), m_activePlot{nullptr}, m_gateItem{nullptr}
{}

void WorkSheetScene::startDrawingGate(GateType gateType)
{
    if (m_drawState == DrawingState::DrawingIdle) {
        m_drawState = DrawingState::DrawingStarted;
        m_gateType = gateType;
    }
}

void WorkSheetScene::finishDrawingGate(bool ok)
{
    if (!ok && m_gateItem) {
        removeItem(m_gateItem);
        delete m_gateItem;
    } else {
        m_gateItems.append(m_gateItem);
    }
    m_gateItem = nullptr;
    m_activePlot = nullptr;
    m_startPosInPlot = QPointF();
    m_gateType = GateType::UnknownGate;
    m_drawState = DrawingState::DrawingIdle;
}

void WorkSheetScene::addNewPlot(PlotType plotType, const Plot &plot)
{
    qDebug() << "Add New Plot, type: " << Plot::plotTypeToString(plotType)
    << "X Axis: " << plot.axisXDetectorId() << MeasurementTypeHelper::measurementTypeToString(plot.xMeasurementType())
    << "Y Axis: " << plot.axisYDetectorId() << MeasurementTypeHelper::measurementTypeToString(plot.yMeasurementType());
    HistogramPlot *histogramPlot = nullptr;
    ScatterPlot *scatterPlot = nullptr;
    switch (plotType) {
    case PlotType::HISTOGRAM_PLOT:
        histogramPlot = new HistogramPlot(plot);
        if (histogramPlot) {
            addItem(histogramPlot);
            m_plots.append(static_cast<PlotBase*>(histogramPlot));
        }
        break;
    case PlotType::SCATTER_PLOT:
        scatterPlot = new ScatterPlot(plot);
        if (scatterPlot) {
            addItem(scatterPlot);
            m_plots.append(static_cast<PlotBase*>(scatterPlot));
        }
        break;
    case PlotType::CONTOUR_PLOT:
        break;
    default:
        break;
    }
    m_plots.last()->setPos(((m_plots.size()-1) % 3) * Plot::defaultPlotSize, ((m_plots.size()-1) / 3) * Plot::defaultPlotSize);
}


void WorkSheetScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch (m_drawState) {
    case DrawingState::DrawingStarted: {
        QGraphicsItem *clickedItem = itemAt(event->scenePos(), QTransform());
        PlotBase *plotItem = qgraphicsitem_cast<PlotBase*>(clickedItem);
        if (plotItem &&  plotItem->isInPlotArea(plotItem->mapFromScene(event->scenePos()))) {
            m_activePlot = plotItem;
            m_activePlot->setFlag(QGraphicsItem::ItemIsMovable, false);
            m_drawState = DrawingState::DrawingInProgress;
            m_startPosInPlot = m_activePlot->mapFromScene(event->scenePos());
            m_gateItem = GateItemFactory::createGateItem(m_gateType, m_startPosInPlot, m_activePlot);
            m_gateItem->setGateName(QString("P%1").arg(m_gateItems.size()+1));
        }
    }
    break;
    case DrawingState::DrawingInProgress: {
        PolygonGateItem *polygonGateItem = qgraphicsitem_cast<PolygonGateItem*>(m_gateItem);
        if (m_gateType == GateType::PolygonGate && polygonGateItem) {
            QPointF pos = m_activePlot->limitScenePointInPlot(event->scenePos());
            polygonGateItem->updatePolygon(pos);
        }
        break;
    }
    case DrawingState::DrawingFinished:
    default:
        break;
    }
    QGraphicsScene::mousePressEvent(event);
}

void WorkSheetScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_drawState == DrawingState::DrawingInProgress && m_activePlot) {
        QPointF pos = m_activePlot->limitScenePointInPlot(event->scenePos());
        m_gateItem->updateGatePreview(pos);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void WorkSheetScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_drawState == DrawingState::DrawingInProgress && m_gateType != GateType::PolygonGate) {
        m_drawState = DrawingState::DrawingFinished;
        QPointF pos = m_activePlot->limitScenePointInPlot(event->scenePos());
        m_gateItem->finishDrawing(pos);
        m_activePlot->setFlag(QGraphicsItem::ItemIsMovable, true);
        emit finishedDrawingGate(m_gateItem);
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void WorkSheetScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_drawState == DrawingState::DrawingInProgress && m_gateType == GateType::PolygonGate) {
        PolygonGateItem *polygonGateItem = qgraphicsitem_cast<PolygonGateItem*>(m_gateItem);
        if (polygonGateItem) {
            QPointF pos = m_activePlot->limitScenePointInPlot(event->scenePos());
            polygonGateItem->finishDrawing(pos);
            m_activePlot->setFlag(QGraphicsItem::ItemIsMovable, true);
            emit finishedDrawingGate(m_gateItem);
        }
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}


bool WorkSheetScene::segmentsIntersect(const QPointF &p1, const QPointF &p2, const QPointF &q1, const QPointF &q2)
{
    auto orientation = [](const QPointF &a, const QPointF &b, const QPointF &c) {
        double cross = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
        if (qFuzzyCompare(cross, 0.0)) return 0;
        return (cross>0) ? 1 : -1;
    };

    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);
    if (o1 != o2 && o3 != o4) return true;
    return false;
}


