#include "MeasurementTypeHelper.h"

QVector<MeasurementType> MeasurementTypeHelper::typeList = {MeasurementType::Height, MeasurementType::Width, MeasurementType::Area};


const std::unordered_map<QString, MeasurementType> MeasurementTypeHelper::stringToEnumMap = {
    {"Height", MeasurementType::Height},
    {"Width", MeasurementType::Width},
    {"Area", MeasurementType::Area},
    {"Unknown", MeasurementType::Unknown}
};

const std::unordered_map<MeasurementType, QString> MeasurementTypeHelper::enumToStringMap = {
    {MeasurementType::Height, "Height"},
    {MeasurementType::Width, "Width"},
    {MeasurementType::Area, "Area"},
    {MeasurementType::Unknown, "Unknown"}
};


// 字符串转换为枚举值
MeasurementType MeasurementTypeHelper::stringToMeasurementType(const QString& str) {
    auto it = stringToEnumMap.find(str);
    if (it != stringToEnumMap.end()) {
        return it->second;
    }
    return MeasurementType::Unknown; // 如果找不到匹配值，返回 Unknown
}

// 枚举值转换为字符串
QString MeasurementTypeHelper::measurementTypeToString(MeasurementType type) {
    auto it = enumToStringMap.find(type);
    if (it != enumToStringMap.end()) {
        return it->second;
    }
    return "Unknown"; // 如果没有匹配值，返回 "Unknown"
}

QString MeasurementTypeHelper::parameterMeasurementType(const QString &parameter, MeasurementType type)
{
    switch (type) {
        case MeasurementType::Height:
            return QString("%1-H").arg(parameter);
        case MeasurementType::Width:
            return QString("%1-W").arg(parameter);
        case MeasurementType::Area:
            return QString("%1-A").arg(parameter);
        default:
            return parameter;
    }
}

int MeasurementTypeHelper::MeasurementTypeIndex(MeasurementType type)
{
    return static_cast<int>(type);
}


