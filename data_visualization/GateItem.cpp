#include "GateItem.h"
#include <QAction>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

GateItem::GateItem(GateType type, const QPointF &startPos, PlotBase *parent)
    : QGraphicsObject(parent), m_parent(parent), m_startPos(startPos), m_drawingFinished(false)
{
    setPos(startPos);
    m_gate = Gate(parent->worksheetId(), "Gate", type, parent->axisXSettingId(), parent->xMeasurementType(),
                  parent->axisYSettingId(), parent->yMeasurementType());
}

GateItem::GateItem(const Gate &gate, PlotBase *parent)
    : QGraphicsObject(parent), m_parent(parent), m_gate(gate), m_drawingFinished(true)
{

}

void GateItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *deleteAction = menu.addAction(QString("Delete Gate-%1").arg(getGateName()));
    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == deleteAction) {
        emit gateDeleteRequested(this);
    }

    event->accept();
}

