#ifndef DETECTORSETTINGSDAO_H
#define DETECTORSETTINGSDAO_H

#include "BaseDAO.h"
#include "DetectorSettings.h"

class DetectorSettingsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit DetectorSettingsDAO(QObject *parent = nullptr);

    bool insertDetectorSettings(const DetectorSettings &detectorSettings);
    bool updateDetectorSettings(const DetectorSettings &detectorSettings);
    bool deleteDetectorSettings(int config_id, int detectorSettingsId);
    QList<DetectorSettings> fetchDetectorSettings(int configId) const;
    DetectorSettings fetchDetectorSettings(int configId, int detectorId) const;
    bool isDetectorSettingsExists(int configId, int detectorId) const;
    bool isDetectorSettingsExists(int configId, const QString &parameter) const;
};

#endif // DETECTORSETTINGSDAO_H
