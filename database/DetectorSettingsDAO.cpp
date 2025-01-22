#include "DetectorSettingsDAO.h"

DetectorSettingsDAO::DetectorSettingsDAO(QObject *parent)
    : BaseDAO{parent}
{}

bool DetectorSettingsDAO::insertDetectorSettings(const DetectorSettings &detectorSettings)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO DetectorSettings (config_id, detector_id, parameter, detector_gain, detector_offset, enable_threshold, threshold_value, enable_height, enable_width, enable_area) "
                  "VALUES (:config_id, :detector_id, :parameter, :detector_gain, :detector_offset, :enable_threshold, :threshold_value, :enable_height, :enable_width, :enable_area)");
    query.bindValue(":config_id", detectorSettings.configId());
    query.bindValue(":detector_id", detectorSettings.detectorId());
    query.bindValue(":parameter", detectorSettings.parameter());
    query.bindValue(":detector_gain", detectorSettings.detectorGain());
    query.bindValue(":detector_offset", detectorSettings.detectorOffset());
    query.bindValue(":enable_threshold", detectorSettings.isEnabledThreshold());
    query.bindValue(":threshold_value", detectorSettings.thresholdValue());
    query.bindValue(":enable_height", detectorSettings.isEnabledHeight());
    query.bindValue(":enable_width", detectorSettings.isEnabledWidth());
    query.bindValue(":enable_area", detectorSettings.isEenabledArea());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool DetectorSettingsDAO::updateDetectorSettings(const DetectorSettings &detectorSettings)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE DetectorSettings SET parameter = :parameter, detector_gain = :detector_gain, detector_offset = :detector_offset, enable_threshold = :enable_threshold, threshold_value = :threshold_value, enable_height = :enable_height, enable_width = :enable_width, enable_area = :enable_area WHERE config_id = :config_id AND detector_id = :detector_id");
    query.bindValue(":parameter", detectorSettings.parameter());
    query.bindValue(":detector_gain", detectorSettings.detectorGain());
    query.bindValue(":detector_offset", detectorSettings.detectorOffset());
    query.bindValue(":enable_threshold", detectorSettings.isEnabledThreshold());
    query.bindValue(":threshold_value", detectorSettings.thresholdValue());
    query.bindValue(":enable_height", detectorSettings.isEnabledHeight());
    query.bindValue(":enable_width", detectorSettings.isEnabledWidth());
    query.bindValue(":enable_area", detectorSettings.isEenabledArea());
    query.bindValue(":config_id", detectorSettings.configId());
    query.bindValue(":detector_id", detectorSettings.detectorId());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool DetectorSettingsDAO::deleteDetectorSettings(int config_id, int detectorSettingsId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM DetectorSettings WHERE config_id = :config_id AND detector_id = :detector_id");
    query.bindValue(":config_id", config_id);
    query.bindValue(":detector_id", detectorSettingsId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

QList<DetectorSettings> DetectorSettingsDAO::fetchDetectorSettings(int configId) const
{
    QList<DetectorSettings> detectorSettings;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE config_id = :config_id");
    query.bindValue(":config_id", configId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return detectorSettings;
    }

    while (query.next()) {
        DetectorSettings settings;
        settings.setConfigId(query.value("config_id").toInt());
        settings.setDetectorId(query.value("detector_id").toInt());
        settings.setParameter(query.value("parameter").toString());
        settings.setDetectorGain(query.value("detector_gain").toInt());
        settings.setDetectorOffset(query.value("detector_offset").toInt());
        settings.enableThreshold(query.value("enable_threshold").toBool());
        settings.setThresholdValue(query.value("threshold_value").toInt());
        settings.enableHeight(query.value("enable_height").toBool());
        settings.enableWidth(query.value("enable_width").toBool());
        settings.enableArea(query.value("enable_area").toBool());
        detectorSettings.append(settings);
    }

    return detectorSettings;
}

DetectorSettings DetectorSettingsDAO::fetchDetectorSettings(int configId, int detectorId) const
{
    DetectorSettings settings;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE config_id = :config_id AND detector_id = :detector_id");
    query.bindValue(":config_id", configId);
    query.bindValue(":detector_id", detectorId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return settings;
    }

    if (query.next()) {
        settings.setConfigId(query.value("config_id").toInt());
        settings.setDetectorId(query.value("detector_id").toInt());
        settings.setParameter(query.value("parameter").toString());
        settings.setDetectorGain(query.value("detector_gain").toInt());
        settings.setDetectorOffset(query.value("detector_offset").toInt());
        settings.enableThreshold(query.value("enable_threshold").toBool());
        settings.setThresholdValue(query.value("threshold_value").toInt());
        settings.enableHeight(query.value("enable_height").toBool());
        settings.enableWidth(query.value("enable_width").toBool());
        settings.enableArea(query.value("enable_area").toBool());
    }

    return settings;
}

bool DetectorSettingsDAO::isDetectorSettingsExists(int configId, int detectorId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE config_id = :config_id AND detector_id = :detector_id");
    query.bindValue(":config_id", configId);
    query.bindValue(":detector_id", detectorId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool DetectorSettingsDAO::isDetectorSettingsExists(int configId, const QString &parameter) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE config_id = :config_id AND parameter = :parameter");
    query.bindValue(":config_id", configId);
    query.bindValue(":parameter", parameter);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}


