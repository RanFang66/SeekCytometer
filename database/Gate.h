#ifndef GATE_H
#define GATE_H

#include <QString>
#include <QPointF>
#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "MeasurementTypeHelper.h"

enum class GateType
{
    RectangleGate,
    PolygonGate,
    EllipseGate,
    IntervalGate,
    QuadrantGate,
    UnknownGate,
};

class Gate
{
public:
    Gate();
    explicit Gate(int worksheetId, QString name, GateType type,  int xAxisSettingId,
                  MeasurementType xMeasurementType, int yAxisSettingId, MeasurementType yMeasurementType, const QList<QPointF> &points = QList<QPointF>(), int parentId = 0);

    static GateType stringToGateType(const QString &str);
    static QString  gateTypeToString(GateType type);

    int id() const;
    int worksheetId() const;
    QString name() const;
    GateType gateType() const;
    const QList<QPointF> &points() const;
    QJsonArray pointsJsonArray() const;
    QString     pointsJsonString() const;
    int xAxisSettingId() const;
    int yAxisSettingId() const;
    MeasurementType xMeasurementType() const;
    MeasurementType yMeasurementType() const;
    int parentId() const;

    void setId(int id);
    void setWorksheetId(int worksheetId);
    void setName(const QString &name);
    void setPoints(const QList<QPointF> &points);
    void setPoinst(const QJsonArray &points);
    void setXAxisSettingId(int xAxisSettingId);
    void setYAxisSettingId(int yAxisSettingId);
    void setParentId(int parentId);
    void setXMeasurementType(MeasurementType type);
    void setYMeasurementType(MeasurementType type);
    void setGateType(GateType type);


private:
    int                 m_id;
    int                 m_worksheetId;
    QString             m_name;
    GateType            m_type;
    QList<QPointF>      m_points;
    int                 m_xAxisSettingId;
    int                 m_yAxisSettingId;
    MeasurementType     m_xMeasurementType;
    MeasurementType     m_yMeasurementType;
    int                 m_parentId;

};


inline int Gate::id() const
{
    return m_id;
}


inline int Gate::worksheetId() const
{
    return m_worksheetId;
}

inline QString Gate::name() const
{
    return m_name;
}

inline GateType Gate::gateType() const
{
    return m_type;
}

inline const QList<QPointF> &Gate::points() const
{
    return m_points;
}

inline int Gate::xAxisSettingId() const
{
    return m_xAxisSettingId;
}

inline int Gate::yAxisSettingId() const
{
    return m_yAxisSettingId;
}

inline MeasurementType Gate::xMeasurementType() const
{
    return m_xMeasurementType;
}

inline MeasurementType Gate::yMeasurementType() const
{
    return m_yMeasurementType;
}

inline int Gate::parentId() const
{
    return m_parentId;
}

inline void Gate::setId(int id)
{
    m_id = id;
}

inline void Gate::setWorksheetId(int worksheetId)
{
    m_worksheetId = worksheetId;
}

inline void Gate::setName(const QString &name)
{
    m_name = name;
}

inline void Gate::setPoints(const QList<QPointF> &points)
{
    m_points = points;
}

inline void Gate::setXAxisSettingId(int xAxisSettingId)
{
    m_xAxisSettingId = xAxisSettingId;
}

inline void Gate::setYAxisSettingId(int yAxisSettingId)
{
    m_yAxisSettingId = yAxisSettingId;
}

inline void Gate::setParentId(int parentId)
{
    m_parentId = parentId;
}

inline void Gate::setXMeasurementType(MeasurementType type)
{
    m_xMeasurementType = type;
}

inline void Gate::setYMeasurementType(MeasurementType type)
{
    m_yMeasurementType = type;
}

inline void Gate::setGateType(GateType type)
{
    m_type = type;
}


#endif // GATE_H
