#ifndef GATEITEMFACTORY_H
#define GATEITEMFACTORY_H

#include "GateItem.h"
#include "IntervalGateItem.h"
#include "QuadrantGateItem.h"
#include "EllipseGateItem.h"
#include "PolygonGateItem.h"
#include "RectangleGateItem.h"

class GateItemFactory
{
public:
    GateItemFactory();

    static GateItem* createGateItem(GateType gateType, const QPointF &startPoint = QPointF(0, 0), PlotBase *parent = nullptr)
    {
        switch (gateType) {
            case GateType::RectangleGate:
                return new RectangleGateItem(startPoint, parent);
            case GateType::EllipseGate:
                return new EllipseGateItem(startPoint, parent);
            case GateType::PolygonGate:
                return new PolygonGateItem(startPoint, parent);
            case GateType::IntervalGate:
                return new IntervalGateItem(startPoint, parent);
            case GateType::QuadrantGate:
                return new QuadrantGateItem(startPoint, parent);
            default:
                return nullptr;
        }
    }
};

#endif // GATEITEMFACTORY_H
