#ifndef GATEITEM_H
#define GATEITEM_H

#include <QGraphicsObject>
#include "Gate.h"

#include "PlotBase.h"



class GateItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit GateItem(const QPointF &m_startPos, PlotBase *parent = nullptr);

    const Gate  &gate() const { return m_gate; }
    void    setGateName(const QString &name) { m_gate.setName(name); }
    void    setGateId(int id) { m_gate.setId(id);}

    virtual void        updateGatePreview(const QPointF &point) = 0;
    virtual void        finishDrawing(const QPointF &point) = 0;
    virtual GateType    gateType() const = 0;
    virtual void        updateGateData() = 0;

protected:
    Gate        m_gate;
    QPointF     m_startPos;
    QPointF     m_previewPos;
    PlotBase    *m_parent;
    bool        m_drawingFinished;
};


#endif // GATEITEM_H
