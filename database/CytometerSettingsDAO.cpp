#include "CytometerSettingsDAO.h"

CytometerSettingsDAO::CytometerSettingsDAO(QObject *parent)
    : BaseDAO{parent}
{}

bool CytometerSettingsDAO::insertCytometerSettings(const CytometerSettings &cytometerSettings)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO CytometerSettings (config_name, parent_type, parent_id, threshold_op) VALUES (:config_name, :parent_type, :parent_id, :threshold_op)");
    query.bindValue(":config_name", cytometerSettings.name());
    query.bindValue(":parent_type", static_cast<int>(cytometerSettings.parentType()));
    query.bindValue(":parent_id", cytometerSettings.parentId());
    query.bindValue(":threshold_op", static_cast<int>(cytometerSettings.thresholdType()));

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

bool CytometerSettingsDAO::updateCytometerSettings(const CytometerSettings &cytometerSettings)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE CytometerSettings SET config_name = :config_name, parent_type = :parent_type, parent_id = :parent_id, threshold_op = :threshold_op WHERE config_id = :config_id");
    query.bindValue(":config_name", cytometerSettings.name());
    query.bindValue(":parent_type", static_cast<int>(cytometerSettings.parentType()));
    query.bindValue(":parent_id", cytometerSettings.parentId());
    query.bindValue(":threshold_op", static_cast<int>(cytometerSettings.thresholdType()));
    query.bindValue(":config_id", cytometerSettings.id());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool CytometerSettingsDAO::deleteCytometerSettings(int cytometerSettingsId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM CytometerSettings WHERE config_id = :config_id");
    query.bindValue(":config_id", cytometerSettingsId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

QList<CytometerSettings> CytometerSettingsDAO::fetchCytometerSettings() const
{
    QList<CytometerSettings> cytometerSettings;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM CytometerSettings");

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return cytometerSettings;
    }

    while (query.next()) {
        CytometerSettings::ParentType parentType = static_cast<CytometerSettings::ParentType>(query.value("parent_type").toInt());
        CytometerSettings::ThresholdType thresholdType = static_cast<CytometerSettings::ThresholdType>(query.value("threshold_op").toInt());
        CytometerSettings settings{query.value("config_id").toInt(), parentType, query.value("parent_id").toInt(), thresholdType, query.value("config_name").toString()};
        cytometerSettings.append(settings);
    }

    return cytometerSettings;
}

CytometerSettings CytometerSettingsDAO::fetchCytometerSettings(int cytometerSettingsId) const
{
    CytometerSettings settings;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM CytometerSettings WHERE config_id = :config_id");
    query.bindValue(":config_id", cytometerSettingsId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return settings;
    }

    if (query.next()) {
        CytometerSettings::ParentType parentType = static_cast<CytometerSettings::ParentType>(query.value("parent_type").toInt());
        CytometerSettings::ThresholdType thresholdType = static_cast<CytometerSettings::ThresholdType>(query.value("threshold_op").toInt());
        settings = CytometerSettings{query.value("config_id").toInt(), parentType, query.value("parent_id").toInt(), thresholdType, query.value("config_name").toString()};
    }

    return settings;
}

bool CytometerSettingsDAO::isCytometerSettingsExists(int cytometerSettingsId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM CytometerSettings WHERE config_id = :config_id");
    query.bindValue(":config_id", cytometerSettingsId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool CytometerSettingsDAO::isCytometerSettingsExists(CytometerSettings::ParentType parentType, int parentId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM CytometerSettings WHERE parent_type = :parent_type AND parent_id = :parent_id");
    query.bindValue(":parent_type", static_cast<int>(parentType));
    query.bindValue(":parent_id", parentId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}



