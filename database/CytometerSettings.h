#ifndef CYTOMETERSETTINGS_H
#define CYTOMETERSETTINGS_H


#include <QList>
#include <QMap>
#include <QString>

#include "DetectorSettings.h"

class CytometerSettings
{
public:
    enum class ParentType
    {
        Experiment,
        Specimen,
        Tube,
    };

    enum class ThresholdType
    {
        ThresholdSingle,
        ThresholdOr,
        ThresholdAnd,
    };


    explicit CytometerSettings();
    explicit CytometerSettings(int id, ParentType type, int parentId, ThresholdType thresholdType, QString name = "CytometerSettings");
    int id() const;
    QString name() const;
    ParentType parentType() const;
    int parentId() const;
    ThresholdType thresholdType() const;


    void addDetectorSettings(const DetectorSettings &detectorSettings);
    void removeDetectorSettings(int detectorId);
    QList<DetectorSettings> detectorSettings() const;


    void setId(int id);
    void setName(const QString &name);
    void setParentType(ParentType parentType);
    void setThresholdType(ThresholdType thresholdType);



private:
    int     m_id;
    QString m_name;
    ParentType m_parentType;
    int m_parentId;
    ThresholdType m_thresholdType;
    QMap<int, DetectorSettings> m_detectorSettings;
};


inline int CytometerSettings::id() const
{
    return m_id;
}

inline QString CytometerSettings::name() const
{
    return m_name;
}

inline CytometerSettings::ParentType CytometerSettings::parentType() const
{
    return m_parentType;
}

inline int CytometerSettings::parentId() const
{
    return m_parentId;
}

inline CytometerSettings::ThresholdType CytometerSettings::thresholdType() const
{
    return m_thresholdType;
}

inline void CytometerSettings::addDetectorSettings(const DetectorSettings &detectorSettings)
{
    m_detectorSettings.insert(detectorSettings.detectorId(), detectorSettings);
}

inline void CytometerSettings::removeDetectorSettings(int detectorId)
{
    m_detectorSettings.remove(detectorId);
}

inline QList<DetectorSettings> CytometerSettings::detectorSettings() const
{
    return m_detectorSettings.values();
}

inline void CytometerSettings::setThresholdType(ThresholdType thresholdType)
{
    m_thresholdType = thresholdType;
}

inline void CytometerSettings::setId(int id)
{
    m_id = id;
}

inline void CytometerSettings::setName(const QString &name)
{
    m_name = name;
}

inline void CytometerSettings::setParentType(ParentType parentType)
{
    m_parentType = parentType;
}


#endif // CYTOMETERSETTINGS_H
