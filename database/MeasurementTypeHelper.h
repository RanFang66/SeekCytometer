#ifndef MEASUREMENTTYPEHELPER_H
#define MEASUREMENTTYPEHELPER_H

#include <QString>
#include <QMetaType>
#include <unordered_map>
#include <QHash>

enum class MeasurementType
{
    Height,
    Width,
    Area,
    Unknown
};
Q_DECLARE_METATYPE(MeasurementType);


class MeasurementTypeHelper
{
public:
    static MeasurementType stringToMeasurementType(const QString &str);
    static QString measurementTypeToString(MeasurementType type);
    static QString parameterMeasurementType(const QString &parameter, MeasurementType type);
    static bool isValidMeasurementType(MeasurementType type);
    static int MeasurementTypeIndex(MeasurementType type);
    static const QVector<MeasurementType> &measurementTypeList();

private:
    MeasurementTypeHelper() = delete;
    static QVector<MeasurementType> typeList;
    static const std::unordered_map<QString, MeasurementType> stringToEnumMap;
    static const std::unordered_map<MeasurementType, QString> enumToStringMap;
};

inline const QVector<MeasurementType> &MeasurementTypeHelper::measurementTypeList()
{
    return typeList;
}

#endif // MEASUREMENTTYPEHELPER_H
