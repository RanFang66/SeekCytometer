#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantMap>

#include "TreeNode.h"
#include "User.h"
#include "Experiment.h"
#include "Specimen.h"
#include "Tube.h"
#include "DetectorSettings.h"
#include "CytometerSettings.h"


class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    static QSqlDatabase getDatabase() {
        static QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
        if (!db.isOpen()) {
            db.setHostName("localhost");
            db.setPort(5432);
            db.setUserName("postgres");
            db.setPassword("kissfire");
            db.setDatabaseName("SeekCytometer");
            if (!db.open()) {
                qWarning() << "Database connection failed" << db.lastError().text();
            }
        }
        return db;
    }

    static DatabaseManager& getInstance()
    {
        static DatabaseManager instance;
        return instance;
    }

    DatabaseManager(DatabaseManager const&) = delete;
    void operator=(DatabaseManager const&) = delete;
    bool connectToDatabase();

    // Query functions
    bool isUserAdmin(int userId) const;
    QList<User> fetchUsers() const;
    QList<Experiment> fetchExperiments(int userId = 0) const;
    QList<Specimen> fetchSpecimens(int experimentId = 0) const;
    QList<Tube> fetchTubes(int tubeId = 0) const;
    QList<DetectorSettings> fetchDetectorSettings(int configId = 0) const;
    CytometerSettings fetchCytometerSettings(CytometerSettings::ParentType parentType, int parentId) const;


    // Modified functions
    bool updateUser(const User &user);
    bool updateExperiment(const Experiment &experiment);
    bool updateSpecimen(const Specimen &specimen);
    bool updateTube(const Tube &tube);

    // Delete functions
    bool deleteUser(int userId);
    bool deleteExperiment(int experimentId);
    bool deleteSpecimen(int specimenId);
    bool deleteTube(int tubeId);
    bool deleteDetectorSettings(int config_id, int detectorSettingsId);
    bool deleteCytometerSettings(CytometerSettings::ParentType parentType, int parentId);

    // Insert functions
    bool insertUser(const User &user);
    bool insertExperiment(const Experiment &experiment);
    bool insertSpecimen(const Specimen &specimen);
    bool insertTube(const Tube &tube);
    bool insertDetectorSettings(int config_id, const DetectorSettings &detectorSettings);
    bool insertCytometerSettings(const CytometerSettings &cytometerSettings);


    TreeNode *getTreeModel(const User &user);


signals:
    // Error handling: emit signals to handle errors
    void databaseError(const QSqlError &error) const;


private:
    QSqlDatabase m_db;
    QSqlError    m_lastError;

    explicit DatabaseManager(QObject *parent = nullptr);
    void databaseErrorOccurred(const QString functionName, const QSqlError &error) const;
};

#endif // DATABASEMANAGER_H
