#include "CytometerSettingsDAO.h"

CytometerSettingsDAO::CytometerSettingsDAO(QObject *parent)
    : BaseDAO{parent}
{}

bool CytometerSettingsDAO::insertCytometerSettings(const CytometerSettings &cytometerSettings)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO CytometerSettings (setting_name, parent_type, experiment_id, specimen_id, tube_id, threshold_op) "
                  "VALUES (:setting_name, :parent_type, :experiment_id, :specimen_id, :tube_id, :threshold_op)");

    query.bindValue(":setting_name", cytometerSettings.name());
    query.bindValue(":parent_type", NodeTypeHelper::nodeTypeToString(cytometerSettings.parentType()));
    query.bindValue(":threshold_op", CytometerSettings::thresholdTypeToString(cytometerSettings.thresholdType()));

    // 确保正确设置 parent_id
    query.bindValue(":experiment_id", cytometerSettings.parentType() == NodeType::Experiment ? cytometerSettings.parentId() : QVariant());
    query.bindValue(":specimen_id", cytometerSettings.parentType() == NodeType::Specimen ? cytometerSettings.parentId() : QVariant());
    query.bindValue(":tube_id", cytometerSettings.parentType() == NodeType::Tube ? cytometerSettings.parentId() : QVariant());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

bool CytometerSettingsDAO::insertCytometerSettings(const QString &name, NodeType parentType, int parentId, ThresholdType thresholdType)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO CytometerSettings (setting_name, parent_type, experiment_id, specimen_id, tube_id, threshold_op) "
                  "VALUES (:setting_name, :parent_type, :experiment_id, :specimen_id, :tube_id, :threshold_op)");

    query.bindValue(":setting_name", name);
    query.bindValue(":parent_type", NodeTypeHelper::nodeTypeToString(parentType));
    query.bindValue(":threshold_op", CytometerSettings::thresholdTypeToString(thresholdType));

    // 只给对应的 parent 字段赋值
    query.bindValue(":experiment_id", parentType == NodeType::Experiment ? parentId : QVariant());
    query.bindValue(":specimen_id", parentType == NodeType::Specimen ? parentId : QVariant());
    query.bindValue(":tube_id", parentType == NodeType::Tube ? parentId : QVariant());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

bool CytometerSettingsDAO::updateCytometerSettings(const CytometerSettings &cytometerSettings)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE CytometerSettings SET setting_name = :setting_name, parent_type = :parent_type, "
                  "experiment_id = :experiment_id, specimen_id = :specimen_id, tube_id = :tube_id, threshold_op = :threshold_op "
                  "WHERE setting_id = :setting_id");

    query.bindValue(":setting_name", cytometerSettings.name());
    query.bindValue(":parent_type", NodeTypeHelper::nodeTypeToString(cytometerSettings.parentType()));
    query.bindValue(":threshold_op", CytometerSettings::thresholdTypeToString(cytometerSettings.thresholdType()));
    query.bindValue(":setting_id", cytometerSettings.id());

    // 只更新对应的 parent 字段
    query.bindValue(":experiment_id", cytometerSettings.parentType() == NodeType::Experiment ? cytometerSettings.parentId() : QVariant());
    query.bindValue(":specimen_id", cytometerSettings.parentType() == NodeType::Specimen ? cytometerSettings.parentId() : QVariant());
    query.bindValue(":tube_id", cytometerSettings.parentType() == NodeType::Tube ? cytometerSettings.parentId() : QVariant());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool CytometerSettingsDAO::deleteCytometerSettings(int cytometerSettingsId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM CytometerSettings WHERE setting_id = :setting_id");
    query.bindValue(":setting_id", cytometerSettingsId);

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
        NodeType parentType = NodeTypeHelper::stringToNodeType(query.value("parent_type").toString());
        ThresholdType thresholdType = CytometerSettings::stringToThresholdType(query.value("threshold_op").toString());

        // 根据 parentType 选择正确的 parent_id
        int parentId = 0;
        if (parentType == NodeType::Experiment) {
            parentId = query.value("experiment_id").toInt();
        } else if (parentType == NodeType::Specimen) {
            parentId = query.value("specimen_id").toInt();
        } else if (parentType == NodeType::Tube) {
            parentId = query.value("tube_id").toInt();
        }

        CytometerSettings settings{query.value("setting_id").toInt(), parentType, parentId, thresholdType, query.value("setting_name").toString()};
        cytometerSettings.append(settings);
    }

    return cytometerSettings;
}

CytometerSettings CytometerSettingsDAO::fetchCytometerSettings(int cytometerSettingsId) const
{
    CytometerSettings settings;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM CytometerSettings WHERE setting_id = :setting_id");
    query.bindValue(":setting_id", cytometerSettingsId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return settings;
    }

    if (query.next()) {
        NodeType parentType = NodeTypeHelper::stringToNodeType(query.value("parent_type").toString());
        ThresholdType thresholdType = CytometerSettings::stringToThresholdType(query.value("threshold_op").toString());

        int parentId = 0;
        if (parentType == NodeType::Experiment) {
            parentId = query.value("experiment_id").toInt();
        } else if (parentType == NodeType::Specimen) {
            parentId = query.value("specimen_id").toInt();
        } else if (parentType == NodeType::Tube) {
            parentId = query.value("tube_id").toInt();
        }

        settings = CytometerSettings{query.value("setting_id").toInt(), parentType, parentId, thresholdType, query.value("setting_name").toString()};
    }

    return settings;
}

bool CytometerSettingsDAO::isCytometerSettingsExists(int cytometerSettingsId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM CytometerSettings WHERE setting_id = :setting_id");
    query.bindValue(":setting_id", cytometerSettingsId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool CytometerSettingsDAO::isCytometerSettingsExists(NodeType parentType, int parentId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM CytometerSettings WHERE parent_type = :parent_type AND experiment_id = :experiment_id AND specimen_id = :specimen_id AND tube_id = :tube_id");
    query.bindValue(":parent_type", NodeTypeHelper::nodeTypeToString(parentType));
    query.bindValue(":experiment_id", parentType == NodeType::Experiment ? parentId : QVariant());
    query.bindValue(":specimen_id", parentType == NodeType::Specimen ? parentId : QVariant());
    query.bindValue(":tube_id", parentType == NodeType::Tube ? parentId : QVariant());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}



