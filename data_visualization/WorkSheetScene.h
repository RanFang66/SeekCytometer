#ifndef WORKSHEETSCENE_H
#define WORKSHEETSCENE_H

#include <QGraphicsScene>
#include "GateItem.h"
#include "PlotBase.h"


enum class DrawingState {
    DrawingIdle,
    DrawingStarted,
    DrawingInProgress,
    DrawingFinished,
};

class WorkSheetScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit WorkSheetScene(QObject *parent = nullptr);
    void startDrawingGate(GateType gateType);
    bool isDrawingGate() const { return m_drawState != DrawingState::DrawingIdle; }
    void finishDrawingGate(bool ok);
    PlotBase* addNewPlot(PlotType plotType, const Plot &plot);
    void addNewGate(GateType gateType, const Gate &gate, PlotBase *parent);

    QList<PlotBase*> plots() const { return m_plots; }
    QList<GateItem*> gates() const {return m_gateItems;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;


signals:
    void finishedDrawingGate(GateItem *gateItem);

public slots:
    void onDeletePlot(PlotBase *plot);
    void onDeleteGate(GateItem *gate);

private:
    DrawingState    m_drawState;
    GateType        m_gateType;
    PlotBase        *m_activePlot;
    QVector<PlotBase*>  m_plots;
    QList<GateItem*>    m_gateItems;

    GateItem        *m_gateItem;
    QPointF         m_startPosInPlot;

    void startDrawing();
    void endDrawing();
    void updateDrawing(QPointF pos);
    bool segmentsIntersect(const QPointF &p1, const QPointF &p2, const QPointF &q1, const QPointF &q2);
};

#endif // WORKSHEETSCENE_H
