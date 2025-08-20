#include "Gate.h"
#include "DetectorSettingsDAO.h"

Gate::Gate()
{
    m_id = 0;
    m_worksheetId = 0;
    m_name = "";
    m_type = GateType::UnknownGate;
    m_xAxisSettingId = 0;
    m_yAxisSettingId = 0;
    m_parentId = 0;
    m_xMeasurementType = MeasurementType::Unknown;
    m_yMeasurementType = MeasurementType::Unknown;
    m_points = QList<QPoint>();
}

Gate::Gate(int worksheetId, QString name, GateType type, int xAxisSettingId, MeasurementType xMeasurementType, int yAxisSettingId, MeasurementType yMeasurementType, const QList<QPoint> &points,  int parentId)
    : m_id(0), m_worksheetId(worksheetId), m_name(name), m_type(type), m_points(points),
    m_xAxisSettingId(xAxisSettingId), m_yAxisSettingId(yAxisSettingId), m_xMeasurementType(xMeasurementType),
    m_yMeasurementType(yMeasurementType), m_parentId(parentId)
{
    m_xAxisSetting = DetectorSettingsDAO().fetchDetectorSettings(xAxisSettingId);
    m_yAxisSetting = DetectorSettingsDAO().fetchDetectorSettings(yAxisSettingId);
}

GateType Gate::stringToGateType(const QString &str)
{
    if (str == "rectangle") {
        return GateType::RectangleGate;
    } else if (str == "polygon") {
        return GateType::PolygonGate;
    } else if (str == "ellipse") {
        return GateType::EllipseGate;
    } else if (str == "interval") {
        return GateType::IntervalGate;
    } else if (str == "quadrant") {
        return GateType::QuadrantGate;
    } else {
        return GateType::UnknownGate;
    }
}

QString Gate::gateTypeToString(GateType type)
{
    switch (type) {
    case GateType::RectangleGate:
        return "rectangle";
    case GateType::PolygonGate:
        return "polygon";
    case GateType::EllipseGate:
        return "ellipse";
    case GateType::IntervalGate:
        return "interval";
    case GateType::QuadrantGate:
        return "quadrant";
    default:
        return "unknown";
    }
}

void Gate::setPoints(const QList<QPointF> &points)
{
    // m_points = points;
    m_points.clear();
    for (const auto &point : points) {
        m_points.append(point.toPoint());
    }
}


QJsonArray Gate::pointsJsonArray() const
{
    QJsonArray array;
    for (const auto &point : m_points) {
        array.append(QJsonObject{
            {"x", point.x()},
            {"y", point.y()}
        });
    }
    return array;
}

QString Gate::pointsJsonString() const
{
    QJsonArray array;
    for (const auto &point : m_points) {
        array.append(QJsonObject{
            {"x", point.x()},
            {"y", point.y()}
        });
    }
    QByteArray jsonData = QJsonDocument(array).toJson(QJsonDocument::Compact);
    return QString::fromUtf8(jsonData);
}





void Gate::setXAxisSettingId(int xAxisSettingId)
{
    m_xAxisSettingId = xAxisSettingId;
    m_xAxisSetting = DetectorSettingsDAO().fetchDetectorSettings(xAxisSettingId);
}


void Gate::setYAxisSettingId(int yAxisSettingId)
{
    m_yAxisSettingId = yAxisSettingId;
    m_yAxisSetting = DetectorSettingsDAO().fetchDetectorSettings(yAxisSettingId);
}




void Gate::setPoinst(const QJsonArray &points)
{
    m_points.clear();
    for (const auto &point : points) {
        m_points.append(QPoint(point.toObject().value("x").toInt(), point.toObject().value("y").toInt()));
    }
}

