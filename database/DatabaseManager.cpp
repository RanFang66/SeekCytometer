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




