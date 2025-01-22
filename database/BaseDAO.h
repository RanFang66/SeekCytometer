#ifndef BASEDAO_H
#define BASEDAO_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>


class BaseDAO : public QObject
{
    Q_OBJECT
public:
    explicit BaseDAO(QObject *parent = nullptr);
    void handleError(const QString functionName, const QSqlQuery &query) const;

protected:
    QSqlDatabase m_db;

signals:
    void databaseError(const QSqlError &error);
};

#endif // BASEDAO_H
