#include "DetectorSettingsDAO.h"

DetectorSettingsDAO::DetectorSettingsDAO(QObject *parent)
    : BaseDAO{parent}
{}

int DetectorSettingsDAO::insertDetectorSettings(const DetectorSettings &detectorSettings)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO DetectorSettings (setting_id, detector_id, parameter_name, detector_gain, detector_offset, enable_threshold, threshold_value, enable_height, enable_width, enable_area) "
                  "VALUES (:setting_id, :detector_id, :parameter_name, :detector_gain, :detector_offset, :enable_threshold, :threshold_value, :enable_height, :enable_width, :enable_area) RETURNING detector_setting_id");
    query.bindValue(":setting_id", detectorSettings.settingId());
    query.bindValue(":detector_id", detectorSettings.detectorId());
    query.bindValue(":parameter_name", detectorSettings.parameterName());
    query.bindValue(":detector_gain", detectorSettings.detectorGain());
    query.bindValue(":detector_offset", detectorSettings.detectorOffset());
    query.bindValue(":enable_threshold", detectorSettings.isEnabledThreshold());
    query.bindValue(":threshold_value", detectorSettings.thresholdValue());
    query.bindValue(":enable_height", detectorSettings.isEnabledHeight());
    query.bindValue(":enable_width", detectorSettings.isEnabledWidth());
    query.bindValue(":enable_area", detectorSettings.isEenabledArea());

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        handleError(__FUNCTION__, query);
        return 0;
    }
}


bool DetectorSettingsDAO::updateDetectorSettings(const DetectorSettings &detectorSettings)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE DetectorSettings SET parameter_name = :parameter_name, detector_gain = :detector_gain, detector_offset = :detector_offset, enable_threshold = :enable_threshold, threshold_value = :threshold_value, enable_height = :enable_height, enable_width = :enable_width, enable_area = :enable_area WHERE setting_id = :setting_id AND detector_id = :detector_id");
    query.bindValue(":parameter_name", detectorSettings.parameterName());
    query.bindValue(":detector_gain", detectorSettings.detectorGain());
    query.bindValue(":detector_offset", detectorSettings.detectorOffset());
    query.bindValue(":enable_threshold", detectorSettings.isEnabledThreshold());
    query.bindValue(":threshold_value", detectorSettings.thresholdValue());
    query.bindValue(":enable_height", detectorSettings.isEnabledHeight());
    query.bindValue(":enable_width", detectorSettings.isEnabledWidth());
    query.bindValue(":enable_area", detectorSettings.isEenabledArea());
    query.bindValue(":setting_id", detectorSettings.settingId());
    query.bindValue(":detector_id", detectorSettings.detectorId());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

bool DetectorSettingsDAO::deleteDetectorSettings(int detectorSettingId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM DetectorSettings WHERE detector_setting_id = :detector_setting_id");
    query.bindValue(":detector_setting_id", detectorSettingId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool DetectorSettingsDAO::deleteDetectorSettings(int settingId, int detectorId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM DetectorSettings WHERE setting_id = :setting_id AND detector_id = :detector_id");
    query.bindValue(":setting_id", settingId);
    query.bindValue(":detector_id", detectorId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

DetectorSettings DetectorSettingsDAO::fetchDetectorSettings(int detectorSettingId) const
{
    DetectorSettings settings;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE detector_setting_id = :detector_setting_id");
    query.bindValue(":detector_setting_id", detectorSettingId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return settings;
    }

    if (query.next()) {
        settings.setId(query.value("detector_setting_id").toInt());
        settings.setSettingId(query.value("setting_id").toInt());
        settings.setDetectorId(query.value("detector_id").toInt());
        settings.setParameterName(query.value("parameter_name").toString());
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

QList<DetectorSettings> DetectorSettingsDAO::fetchDetectorSettingsList(int settingId) const
{
    QList<DetectorSettings> detectorSettings;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE setting_id = :setting_id");
    query.bindValue(":setting_id", settingId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return detectorSettings;
    }

    while (query.next()) {
        DetectorSettings settings;
        settings.setId(query.value("detector_setting_id").toInt());
        settings.setSettingId(query.value("setting_id").toInt());
        settings.setDetectorId(query.value("detector_id").toInt());
        settings.setParameterName(query.value("parameter_name").toString());
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

DetectorSettings DetectorSettingsDAO::fetchDetectorSettings(int settingId, int detectorId) const
{
    DetectorSettings settings;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE setting_id = :setting_id AND detector_id = :detector_id");
    query.bindValue(":setting_id", settingId);
    query.bindValue(":detector_id", detectorId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return settings;
    }

    if (query.next()) {
        settings.setId(query.value("detector_setting_id").toInt());
        settings.setSettingId(query.value("setting_id").toInt());
        settings.setDetectorId(query.value("detector_id").toInt());
        settings.setParameterName(query.value("parameter_name").toString());
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

bool DetectorSettingsDAO::isDetectorSettingsExists(int settingId, int detectorId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE setting_id = :setting_id AND detector_id = :detector_id");
    query.bindValue(":setting_id", settingId);
    query.bindValue(":detector_id", detectorId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool DetectorSettingsDAO::isDetectorSettingsExists(int settingId, const QString &parameterName) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM DetectorSettings WHERE setting_id = :setting_id AND parameter_name = :parameter_name");
    query.bindValue(":setting_id", settingId);
    query.bindValue(":parameter_name", parameterName);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

int DetectorSettingsDAO::getSettingDetectorId(int detectorSettingId)
{
    int id = 0;
    QSqlQuery query(m_db);
    query.prepare("SELECT detector_id FROM DetectorSettings WHERE detector_setting_id = :detector_setting_id");
    query.bindValue(":detector_setting_id", detectorSettingId);
    if (!query.exec() ) {
        handleError(__FUNCTION__, query);
        return 0;
    }
    if (query.next()) {
        id = query.value(0).toInt();
    }
    return id;
}


