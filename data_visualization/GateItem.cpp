#include "GateItem.h"

GateItem::GateItem(const QPointF &startPos, PlotBase *parent)
    : QGraphicsObject(parent), m_parent(parent), m_startPos(startPos), m_drawingFinished(false)
{
    setPos(startPos);
    m_gate = Gate(parent->worksheetId(), "Gate", GateType::PolygonGate, parent->axisXSettingId(), parent->xMeasurementType(),
                  parent->axisYSettingId(), parent->yMeasurementType());
}

