#include "WorkSheetsDAO.h"

WorkSheetsDAO::WorkSheetsDAO(QObject *parent)
    : BaseDAO{parent}
{}

int WorkSheetsDAO::insertWorkSheet(const WorkSheet &workSheet)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO WorkSheets (is_global, experiment_id, tube_id, worksheet_name) "
                  "VALUES (:is_global, :experiment_id, :tube_id, :worksheet_name) RETURNING worksheet_id");
    query.bindValue(":is_global", workSheet.isGlobal());
    query.bindValue(":experiment_id", workSheet.isGlobal() ? workSheet.parentId() : QVariant());
    query.bindValue(":tube_id", workSheet.isGlobal() ? QVariant() : workSheet.parentId());
    query.bindValue(":worksheet_name", workSheet.name());

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        handleError(__FUNCTION__, query);
        return 0;
    }
}


int WorkSheetsDAO::insertWorkSheet(const QString &workSheetName, bool isGlobal, int parentId)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO WorkSheets (is_global, experiment_id, tube_id, worksheet_name) "
                  "VALUES (:is_global, :experiment_id, :tube_id, :worksheet_name) RETURNING worksheet_id");
    query.bindValue(":is_global", isGlobal);
    query.bindValue(":experiment_id", isGlobal ? parentId : QVariant());
    query.bindValue(":tube_id", isGlobal ? QVariant() : parentId);
    query.bindValue(":worksheet_name", workSheetName);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        handleError(__FUNCTION__, query);
        return 0;
    }
}


bool WorkSheetsDAO::updateWorkSheet(const WorkSheet &workSheet)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE WorkSheets SET worksheet_name = :worksheet_name WHERE worksheet_id = :worksheet_id");
    query.bindValue(":worksheet_name", workSheet.name());
    query.bindValue(":worksheet_id", workSheet.id());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool WorkSheetsDAO::deleteWorkSheet(int workSheetId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM WorkSheets WHERE worksheet_id = :worksheet_id");
    query.bindValue(":worksheet_id", workSheetId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


QList<WorkSheet> WorkSheetsDAO::fetchWorkSheets(bool isGlobal, int parentId) const
{
    QList<WorkSheet> workSheets;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM WorkSheets WHERE is_global = :is_global AND "
                  "(experiment_id = :experiment_id OR tube_id = :tube_id)");
    query.bindValue(":is_global", isGlobal);
    query.bindValue(":experiment_id", isGlobal ? parentId : QVariant());
    query.bindValue(":tube_id", isGlobal ? QVariant() : parentId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return workSheets;
    }

    while (query.next()) {
        WorkSheet workSheet;
        workSheet.setId(query.value("worksheet_id").toInt());
        workSheet.setIsGlobal(query.value("is_global").toBool());
        workSheet.setParentId(isGlobal ? query.value("experiment_id").toInt() : query.value("tube_id").toInt());
        workSheet.setName(query.value("worksheet_name").toString());
        workSheets.append(workSheet);
    }

    return workSheets;
}

WorkSheet WorkSheetsDAO::fetchWorkSheet(int workSheetId) const
{
    WorkSheet workSheet;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM WorkSheets WHERE worksheet_id = :worksheet_id");
    query.bindValue(":worksheet_id", workSheetId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return workSheet;
    }

    if (query.next()) {
        workSheet.setId(query.value("worksheet_id").toInt());
        workSheet.setIsGlobal(query.value("is_global").toBool());
        workSheet.setParentId(query.value("experiment_id").toInt());
        workSheet.setName(query.value("worksheet_name").toString());
    } else {
        qDebug() << QString("WorkSheet with id %1 not found!").arg(workSheetId);
    }

    return workSheet;
}


bool WorkSheetsDAO::isWorkSheetExists(int workSheetId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM WorkSheets WHERE worksheet_id = :worksheet_id");
    query.bindValue(":worksheet_id", workSheetId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

bool WorkSheetsDAO::isWorkSheetExists(bool isGlobal, int parentId, const QString &name) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM WorkSheets WHERE is_global = :is_global AND "
                  "(experiment_id = :experiment_id OR tube_id = :tube_id) AND worksheet_name = :worksheet_name");
    query.bindValue(":is_global", isGlobal);
    query.bindValue(":experiment_id", isGlobal ? parentId : QVariant());
    query.bindValue(":tube_id", isGlobal ? QVariant() : parentId);
    query.bindValue(":worksheet_name", name);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}



