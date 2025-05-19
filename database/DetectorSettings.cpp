#include "DetectorSettings.h"

DetectorSettings::DetectorSettings()
:m_id(0), m_settingId(0), m_detectorId{0}, m_parameterName{""}, m_detectorGain{100}, m_detectorOffset{0}, m_enableThreshold{false}, m_thresholdValue{0},
m_enableHeight{true}, m_enableWidth{false}, m_enableArea{false}
{
}

DetectorSettings::DetectorSettings(int configId, int detectorId, QString parameter, int detectorGain, int detectorOffset,
                                   bool enableThreshold, int thresholdVal, bool enableHeight, bool enableWidth, bool enableArea)
:m_id(0), m_settingId(configId), m_detectorId{detectorId}, m_parameterName{parameter}, m_detectorGain{detectorGain}, m_detectorOffset{detectorOffset},
m_enableThreshold{enableThreshold}, m_thresholdValue{thresholdVal},
m_enableHeight{enableHeight}, m_enableWidth{enableWidth}, m_enableArea{enableArea}
{

}

DetectorSettings::DetectorSettings(int settingId, const Detector &detector)
:m_id(0), m_settingId(settingId), m_detectorId{detector.id()}, m_parameterName{detector.name()}, m_detectorGain{detector.defaultGain()}, m_detectorOffset{detector.defaultOffset()},
m_enableThreshold{false}, m_thresholdValue{1000}, m_enableHeight{false}, m_enableWidth{false}, m_enableArea{false}
{

}

QString DetectorSettings::measurementTypeToString(MeasurementType type)
{
    switch (type) {
    case MeasurementType::Height:
        return "Height";
    case MeasurementType::Width:
        return "Width";
    case MeasurementType::Area:
        return "Area";
    default:
        return "Unknown";
    }
}

MeasurementType DetectorSettings::stringToMeasurementType(const QString &type)
{
    if (type == "Height") {
        return MeasurementType::Height;
    } else if (type == "Width") {
        return MeasurementType::Width;
    } else if (type == "Area") {
        return MeasurementType::Area;
    } else {
        return MeasurementType::Unknown;
    }
}

QList<MeasurementType> DetectorSettings::enabledMeasurementTypes() const
{
    QList<MeasurementType> types;
    if (m_enableHeight) {
        types.append(MeasurementType::Height);
    }
    if (m_enableWidth) {
        types.append(MeasurementType::Width);
    }
    if (m_enableArea) {
        types.append(MeasurementType::Area);
    }
    return types;
}


