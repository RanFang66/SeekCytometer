#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent}
{}

void DatabaseManager::databaseErrorOccurred(const QString functionName, const QSqlError &error) const
{
    qWarning() << QString("Database error occured in %1:").arg(functionName) << error.text();
    emit databaseError(error);
}

bool DatabaseManager::connectToDatabase()
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName("localhost");
    m_db.setPort(5432);
    m_db.setUserName("postgres");
    m_db.setPassword("kissfire");

    m_db.setDatabaseName("SeekCytometer");

    if (!m_db.open()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    qDebug() << "Database connected";
    return true;
}

bool DatabaseManager::isUserAdmin(int userId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT user_admin FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }

    if (query.next()) {
        return query.value("user_admin").toBool();
    } else {
        qWarning() << "User not found";
        return false;
    }
}

QList<User> DatabaseManager::fetchUsers() const
{
    QList<User> users;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Users");

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return users;
    }

    while (query.next()) {
        User user{
            query.value("user_id").toInt(),
            query.value("user_name").toString(),
            query.value("user_admin").toBool(),
            query.value("department").toString(),
            query.value("email").toString()
        };
        users.append(user);
    }

    return users;
}

QList<Experiment> DatabaseManager::fetchExperiments(int userId) const
{
    QList<Experiment> experiments;

    QSqlQuery query(m_db);

    if (userId == 0) {
        query.prepare("SELECT * FROM Experiments");
    } else {
        query.prepare("SELECT * FROM Experiments WHERE user_id = :user_id");
        query.bindValue(":user_id", userId);
    }

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return experiments;
    }

    while (query.next()) {
        Experiment experiment{
            query.value("experiment_id").toInt(),
            query.value("experiment_name").toString(),
            query.value("user_id").toInt()
        };
        experiments.append(experiment);
    }

    return experiments;
}

QList<Specimen> DatabaseManager::fetchSpecimens(int experimentId) const
{
    QList<Specimen> specimens;

    QSqlQuery query(m_db);

    if (experimentId == 0) {
        query.prepare("SELECT * FROM Specimens");
    } else {
        query.prepare("SELECT * FROM Specimens WHERE experiment_id = :experiment_id");
        query.bindValue(":experiment_id", experimentId);
    }

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return specimens;
    }

    while (query.next()) {
        Specimen specimen{
            query.value("specimen_id").toInt(),
            query.value("specimen_name").toString(),
            query.value("experiment_id").toInt()
        };
        specimens.append(specimen);
    }

    return specimens;
}


QList<Tube> DatabaseManager::fetchTubes(int specimenId) const
{
    QList<Tube> tubes;

    QSqlQuery query(m_db);
    if (specimenId == 0) {
        query.prepare("SELECT * FROM Tubes");
    } else {
        query.prepare("SELECT * FROM Tubes WHERE specimen_id = :specimen_id");
        query.bindValue(":specimen_id", specimenId);
    }

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return tubes;
    }

    while (query.next()) {
        Tube tube{
            query.value("tube_id").toInt(),
            query.value("tube_name").toString(),
            query.value("specimen_id").toInt()
        };
        tubes.append(tube);
    }

    return tubes;
}

QList<DetectorSettings> DatabaseManager::fetchDetectorSettings(int configId) const
{
    QList<DetectorSettings> detectorSettings;

    QSqlQuery query(m_db);

    query.prepare("SELECT * FROM DetectorSettings WHERE config_id = :config_id");
    query.bindValue(":config_id", configId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return detectorSettings;
    }

    while (query.next()) {
        DetectorSettings settings{
            query.value("config_id").toInt(),
            query.value("detector_id").toInt(),
            query.value("parameter_name").toString(),
            query.value("detector_gain").toInt(),
            query.value("detector_offset").toInt(),
            query.value("enable_threshold").toBool(),
            query.value("threshold_value").toInt(),
            query.value("enable_height").toBool(),
            query.value("enable_width").toBool(),
            query.value("enable_area").toBool()
        };
        detectorSettings.append(settings);
    }

    return detectorSettings;
}

CytometerSettings DatabaseManager::fetchCytometerSettings(CytometerSettings::ParentType parentType, int parentId) const
{
    CytometerSettings settings;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM CytometerSettings WHERE parent_type = :parent_type AND parent_id = :parent_id");
    query.bindValue(":parent_type", static_cast<int>(parentType));
    query.bindValue(":parent_id", parentId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return settings;
    }

    if (query.next()) {
        settings.setId(query.value("config_id").toInt());
        settings.setName(query.value("config_name").toString());
        settings.setParentType(static_cast<CytometerSettings::ParentType>(query.value("parent_type").toInt()));
        settings.setThresholdType(static_cast<CytometerSettings::ThresholdType>(query.value("threshold_type").toInt()));
    }

    return settings;
}

bool DatabaseManager::updateUser(const User &user)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Users SET user_name = :user_name, user_admin = :user_admin, department = :department, email = :email WHERE user_id = :user_id");
    query.bindValue(":user_name", user.name());
    query.bindValue(":user_admin", user.isAdmin());
    query.bindValue(":department", user.department());
    query.bindValue(":email", user.email());
    query.bindValue(":user_id", user.id());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::updateExperiment(const Experiment &experiment)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Experiments SET experiment_name = :experiment_name WHERE experiment_id = :experiment_id");
    query.bindValue(":experiment_name", experiment.name());
    query.bindValue(":experiment_id", experiment.id());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::updateSpecimen(const Specimen &specimen)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Specimens SET specimen_name = :specimen_name WHERE specimen_id = :specimen_id");
    query.bindValue(":specimen_name", specimen.name());
    query.bindValue(":specimen_id", specimen.id());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::updateTube(const Tube &tube)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Tubes SET tube_name = :tube_name WHERE tube_id = :tube_id");
    query.bindValue(":tube_name", tube.name());
    query.bindValue(":tube_id", tube.id());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::deleteUser(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::deleteExperiment(int experimentId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Experiments WHERE experiment_id = :experiment_id");
    query.bindValue(":experiment_id", experimentId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::deleteSpecimen(int specimenId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Specimens WHERE specimen_id = :specimen_id");
    query.bindValue(":specimen_id", specimenId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::deleteTube(int tubeId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Tubes WHERE tube_id = :tube_id");
    query.bindValue(":tube_id", tubeId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::deleteDetectorSettings(int config_id, int detectorSettingsId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM DetectorSettings WHERE config_id = :config_id AND detector_id = :detector_id");
    query.bindValue(":config_id", config_id);
    query.bindValue(":detector_id", detectorSettingsId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::deleteCytometerSettings(CytometerSettings::ParentType parentType, int parentId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM CytometerSettings WHERE parent_type = :parent_type AND parent_id = :parent_id");
    query.bindValue(":parent_type", static_cast<int>(parentType));
    query.bindValue(":parent_id", parentId);

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::insertUser(const User &user)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Users (user_name, user_admin, department, email) VALUES (:user_name, :user_admin, :department, :email)");
    query.bindValue(":user_name", user.name());
    query.bindValue(":user_admin", user.isAdmin());
    query.bindValue(":department", user.department());
    query.bindValue(":email", user.email());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}


bool DatabaseManager::insertExperiment(const Experiment &experiment)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Experiments (experiment_name, user_id) VALUES (:experiment_name, :user_id)");
    query.bindValue(":experiment_name", experiment.name());
    query.bindValue(":user_id", experiment.userId());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::insertSpecimen(const Specimen &specimen)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Specimens (specimen_name, experiment_id) VALUES (:specimen_name, :experiment_id)");
    query.bindValue(":specimen_name", specimen.name());
    query.bindValue(":experiment_id", specimen.experimentId());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::insertTube(const Tube &tube)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Tubes (tube_name, specimen_id) VALUES (:tube_name, :specimen_id)");
    query.bindValue(":tube_name", tube.name());
    query.bindValue(":specimen_id", tube.specimenId());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::insertDetectorSettings(int config_id, const DetectorSettings &detectorSettings)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO DetectorSettings (config_id, detector_id, parameter_name, detector_gain, detector_offset, enable_threshold, threshold_value, enable_height, enable_width, enable_area) "
                  "VALUES (:config_id, :detector_id, :parameter_name, :detector_gain, :detector_offset, :enable_threshold, :threshold_value, :enable_height, :enable_width, :enable_area)");
    query.bindValue(":config_id", config_id);
    query.bindValue(":detector_id", detectorSettings.detectorId());
    query.bindValue(":parameter_name", detectorSettings.parameter());
    query.bindValue(":detector_gain", detectorSettings.detectorGain());
    query.bindValue(":detector_offset", detectorSettings.detectorOffset());
    query.bindValue(":enable_threshold", detectorSettings.isEnabledThreshold());
    query.bindValue(":threshold_value", detectorSettings.thresholdValue());
    query.bindValue(":enable_height", detectorSettings.isEnabledHeight());
    query.bindValue(":enable_width", detectorSettings.isEnabledWidth());
    query.bindValue(":enable_area", detectorSettings.isEenabledArea());

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::insertCytometerSettings(const CytometerSettings &cytometerSettings)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO CytometerSettings (config_name, parent_type, parent_id, threshold_type) VALUES (:config_name, :parent_type, :parent_id, :threshold_type)");
    query.bindValue(":config_name", cytometerSettings.name());
    query.bindValue(":parent_type", static_cast<int>(cytometerSettings.parentType()));
    query.bindValue(":parent_id", cytometerSettings.parentId());
    query.bindValue(":threshold_type", static_cast<int>(cytometerSettings.thresholdType()));

    if (!query.exec()) {
        databaseErrorOccurred(__FUNCTION__, m_db.lastError());
        return false;
    }
    return true;
}


TreeNode *DatabaseManager::getTreeModel(const User &user)
{
    TreeNode *root = new TreeNode(NodeType::Root, 0, "Root", QDateTime::currentDateTime());

    if (isUserAdmin(user.id())) {
        QList<User> users = fetchUsers();
        for (const User &user : users) {
            TreeNode *userNode = new TreeNode(NodeType::User, user.id(), user.name(), user.createdDate(), root);
            root->addChild(userNode);
            QList<Experiment> experiments = fetchExperiments(user.id());
            for (const Experiment &experiment : experiments) {
                TreeNode *experimentNode = new TreeNode(NodeType::Experiment, experiment.id(), experiment.name(), experiment.createdDate(), userNode);
                userNode->addChild(experimentNode);
                QList<Specimen> specimens = fetchSpecimens(experiment.id());
                for (const Specimen &specimen : specimens) {
                    TreeNode *specimenNode = new TreeNode(NodeType::Specimen, specimen.id(), specimen.name(), specimen.createdDate(), experimentNode);
                    experimentNode->addChild(specimenNode);
                    QList<Tube> tubes = fetchTubes(specimen.id());
                    for (const Tube &tube : tubes) {
                        TreeNode *tubeNode = new TreeNode(NodeType::Tube, tube.id(), tube.name(), tube.createdDate(), specimenNode);
                        specimenNode->addChild(tubeNode);
                    }
                }
            }
        }
    } else {
        TreeNode *userNode = new TreeNode(NodeType::User, user.id(), user.name(), user.createdDate(), root);
        root->addChild(userNode);
        QList<Experiment> experiments = fetchExperiments(user.id());
        for (const Experiment &experiment : experiments) {
            TreeNode *experimentNode = new TreeNode(NodeType::Experiment, experiment.id(), experiment.name(), experiment.createdDate(), userNode);
            userNode->addChild(experimentNode);
            QList<Specimen> specimens = fetchSpecimens(experiment.id());
            for (const Specimen &specimen : specimens) {
                TreeNode *specimenNode = new TreeNode(NodeType::Specimen, specimen.id(), specimen.name(), specimen.createdDate(), experimentNode);
                experimentNode->addChild(specimenNode);
                QList<Tube> tubes = fetchTubes(specimen.id());
                for (const Tube &tube : tubes) {
                    TreeNode *tubeNode = new TreeNode(NodeType::Tube, tube.id(), tube.name(), tube.createdDate(), specimenNode);
                    specimenNode->addChild(tubeNode);
                }
            }
        }
    }

    return root;
}



