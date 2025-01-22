#include "BaseDAO.h"
#include "DatabaseManager.h"

BaseDAO::BaseDAO(QObject *parent)
    : QObject{parent}, m_db{DatabaseManager::getDatabase()}
{}

void BaseDAO::handleError(const QString functionName, const QSqlQuery &query) const
{
    QSqlError error = query.lastError();
    if (error.isValid()) {
        qWarning() << QString("Database error occured in %1:").arg(functionName) << error.text();
    }
}
