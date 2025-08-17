#ifndef DETECTORSETTINGSDAO_H
#define DETECTORSETTINGSDAO_H

#include "BaseDAO.h"
#include "DetectorSettings.h"



// SQL for create table
// CREATE TABLE DetectorSettings (
//     setting_id        INT NOT NULL,
//     detector_id      INT NOT NULL,
//     parameter_name   VARCHAR(64) NOT NULL,
//     detector_gain    INT NOT NULL DEFAULT 100,
//     detector_offset  INT NOT NULL DEFAULT 0,
//     enable_threshold BOOLEAN NOT NULL DEFAULT FALSE,
//     threshold_value  INT NOT NULL DEFAULT 1000,
//     enable_height    BOOLEAN NOT NULL DEFAULT FALSE,
//     enable_width     BOOLEAN NOT NULL DEFAULT FALSE,
//     enable_area      BOOLEAN NOT NULL DEFAULT FALSE,
//     FOREIGN KEY (setting_id) REFERENCES CytometerSettings(setting_id) ON DELETE CASCADE,
//     FOREIGN KEY (detector_id) REFERENCES Detectors(detector_id) ON DELETE CASCADE,
//     PRIMARY KEY (setting_id, detector_id)
//     );

class DetectorSettingsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit DetectorSettingsDAO(QObject *parent = nullptr);

    int  insertDetectorSettings(const DetectorSettings &detectorSettings);
    bool updateDetectorSettings(const DetectorSettings &detectorSettings);
    bool deleteDetectorSettings(int detectorSettingId);
    bool deleteDetectorSettings(int settingId, int detectorId);
    QList<DetectorSettings> fetchDetectorSettings(int settingId) const;
    DetectorSettings fetchDetectorSettings(int settingId, int detectorId) const;
    bool isDetectorSettingsExists(int settingId, int detectorId) const;
    bool isDetectorSettingsExists(int settingId, const QString &parameterName) const;

    int getSettingDetectorId(int detectorSettingId);

};

#endif // DETECTORSETTINGSDAO_H
