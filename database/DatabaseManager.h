#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantMap>


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
            } else {
                qDebug() << "Database connected";
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
