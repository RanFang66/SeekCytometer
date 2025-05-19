#ifndef DETECTORSETTINGS_H
#define DETECTORSETTINGS_H

#include <QString>
#include <QMetaType>
#include "Detector.h"
#include <QList>
#include "MeasurementTypeHelper.h"

class DetectorSettings
{
public:
    DetectorSettings();
    explicit DetectorSettings(int settingId, int detectorId, QString parameter, int detectorGain = 100, int detectorOffset = 0,
                              bool enableThreshold = false, int thresholdValue = 0,
                              bool enableHeight = true, bool enableWidth = false, bool enableArea = false);
    explicit DetectorSettings(int settingId, const Detector &detector);

    static QString measurementTypeToString(MeasurementType type);
    static MeasurementType stringToMeasurementType(const QString &type);


    int id() const;
    int settingId() const;
    int detectorId() const;
    QString parameterName() const;
    int detectorGain() const;
    int detectorOffset() const;
    bool isEnabledThreshold() const;
    int thresholdValue() const;
    bool isEnabledHeight() const;
    bool isEnabledWidth() const;
    bool isEenabledArea() const;

    void setId(int id);
    void setSettingId(int settingId);
    void setDetectorId(int detectorId);
    void setParameterName(const QString &parameter);
    void setDetectorGain(int detectorGain);
    void setDetectorOffset(int detectorOffset);
    void enableThreshold(bool enableThreshold);
    void setThresholdValue(int thresholdValue);
    void enableHeight(bool enableHeight);
    void enableWidth(bool enableWidth);
    void enableArea(bool enableArea);
    QList<MeasurementType> enabledMeasurementTypes() const;


private:
    int     m_id;
    int     m_settingId;
    int     m_detectorId;
    QString m_parameterName;
    int     m_detectorGain;
    int     m_detectorOffset;
    bool    m_enableHeight;
    bool    m_enableWidth;
    bool    m_enableArea;
    bool    m_enableThreshold;
    int     m_thresholdValue;
};

inline int DetectorSettings::id() const
{
    return m_id;
}

inline int DetectorSettings::settingId() const
{
    return m_settingId;
}

inline void DetectorSettings::setSettingId(int configId)
{
    m_settingId = configId;
}

inline void DetectorSettings::setDetectorId(int detectorId)
{
    m_detectorId = detectorId;
}

inline int DetectorSettings::detectorId() const
{
    return m_detectorId;
}

inline QString DetectorSettings::parameterName() const
{
    return m_parameterName;
}

inline int DetectorSettings::detectorGain() const
{
    return m_detectorGain;
}

inline int DetectorSettings::detectorOffset() const
{
    return m_detectorOffset;
}

inline bool DetectorSettings::isEnabledThreshold() const
{
    return m_enableThreshold;
}

inline int DetectorSettings::thresholdValue() const
{
    return m_thresholdValue;
}

inline bool DetectorSettings::isEnabledHeight() const
{
    return m_enableHeight;
}

inline bool DetectorSettings::isEnabledWidth() const
{
    return m_enableWidth;
}

inline bool DetectorSettings::isEenabledArea() const
{
    return m_enableArea;
}

inline void DetectorSettings::setId(int id)
{
    m_id = id;
}

inline void DetectorSettings::setParameterName(const QString &parameterName)
{
    m_parameterName = parameterName;
}

inline void DetectorSettings::setDetectorGain(int detectorGain)
{
    m_detectorGain = detectorGain;
}

inline void DetectorSettings::setDetectorOffset(int detectorOffset)
{
    m_detectorOffset = detectorOffset;
}

inline void DetectorSettings::enableThreshold(bool enableThreshold)
{
    m_enableThreshold = enableThreshold;
}

inline void DetectorSettings::setThresholdValue(int thresholdValue)
{
    m_thresholdValue = thresholdValue;
}

inline void DetectorSettings::enableHeight(bool enableHeight)
{
    m_enableHeight = enableHeight;
}

inline void DetectorSettings::enableWidth(bool enableWidth)
{
    m_enableWidth = enableWidth;
}

inline void DetectorSettings::enableArea(bool enableArea)
{
    m_enableArea = enableArea;
}

#endif // DETECTORSETTINGS_H
