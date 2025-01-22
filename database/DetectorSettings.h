#ifndef DETECTORSETTINGS_H
#define DETECTORSETTINGS_H

#include <QString>


class DetectorSettings
{
public:
    explicit DetectorSettings();
    explicit DetectorSettings(int configId, int detectorId, QString parameter, int detectorGain = 100, int detectorOffset = 0,
                              bool enableThreshold = false, int thresholdValue = 0,
                              bool enableHeight = true, bool enableWidth = false, bool enableArea = false);
    int configId() const;
    int detectorId() const;
    QString parameter() const;
    int detectorGain() const;
    int detectorOffset() const;
    bool isEnabledThreshold() const;
    int thresholdValue() const;
    bool isEnabledHeight() const;
    bool isEnabledWidth() const;
    bool isEenabledArea() const;


    void setConfigId(int configId);
    void setDetectorId(int detectorId);
    void setParameter(const QString &parameter);
    void setDetectorGain(int detectorGain);
    void setDetectorOffset(int detectorOffset);
    void enableThreshold(bool enableThreshold);
    void setThresholdValue(int thresholdValue);
    void enableHeight(bool enableHeight);
    void enableWidth(bool enableWidth);
    void enableArea(bool enableArea);


private:
    int     m_configId;
    int     m_detectorId;
    QString m_parameter;
    int     m_detectorGain;
    int     m_detectorOffset;
    bool    m_enableHeight;
    bool    m_enableWidth;
    bool    m_enableArea;
    bool    m_enableThreshold;
    int     m_thresholdValue;
};

inline int DetectorSettings::configId() const
{
    return m_configId;
}

inline void DetectorSettings::setConfigId(int configId)
{
    m_configId = configId;
}

inline void DetectorSettings::setDetectorId(int detectorId)
{
    m_detectorId = detectorId;
}

inline int DetectorSettings::detectorId() const
{
    return m_detectorId;
}

inline QString DetectorSettings::parameter() const
{
    return m_parameter;
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

inline void DetectorSettings::setParameter(const QString &parameter)
{
    m_parameter = parameter;
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
