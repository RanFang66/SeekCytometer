#include "SpecimensDAO.h"

SpecimensDAO::SpecimensDAO(QObject *parent)
    : BaseDAO{parent}
{}


int SpecimensDAO::insertSpecimen(const Specimen &specimen)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Specimens (experiment_id, specimen_name) VALUES (:experiment_id, :specimen_name) RETURNING specimen_id");
    query.bindValue(":experiment_id", specimen.experimentId());
    query.bindValue(":specimen_name", specimen.name());

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        handleError(__FUNCTION__, query);
        return 0;
    }
}

int SpecimensDAO::insertSpecimen(const QString &name, int experimentId)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Specimens (experiment_id, specimen_name) VALUES (:experiment_id, :specimen_name) RETURNING specimen_id");
    query.bindValue(":experiment_id", experimentId);
    query.bindValue(":specimen_name", name);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        handleError(__FUNCTION__, query);
        return 0;
    }
}

bool SpecimensDAO::updateSpecimen(const Specimen &specimen)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Specimens SET specimen_name = :specimen_name WHERE specimen_id = :specimen_id");
    query.bindValue(":specimen_name", specimen.name());
    query.bindValue(":specimen_id", specimen.id());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

bool SpecimensDAO::deleteSpecimen(int specimenId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Specimens WHERE specimen_id = :specimen_id");
    query.bindValue(":specimen_id", specimenId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

QList<Specimen> SpecimensDAO::fetchSpecimens() const
{
    QList<Specimen> specimens;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Specimens");

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return specimens;
    }

    while (query.next()) {
        Specimen specimen;
        specimen.setId(query.value("specimen_id").toInt());
        specimen.setExperimentId(query.value("experiment_id").toInt());
        specimen.setName(query.value("specimen_name").toString());
        specimens.append(specimen);
    }
    return specimens;
}

Specimen SpecimensDAO::fetchSpecimen(int specimenId) const
{
    Specimen specimen;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Specimens WHERE specimen_id = :specimen_id");
    query.bindValue(":specimen_id", specimenId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return specimen;
    }

    if (query.next()) {
        specimen.setId(query.value("specimen_id").toInt());
        specimen.setExperimentId(query.value("experiment_id").toInt());
        specimen.setName(query.value("specimen_name").toString());
    }

    return specimen;
}

bool SpecimensDAO::isSpecimenExists(const QString &name, int experimentId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Specimens WHERE specimen_name = :specimen_name AND experiment_id = :experiment_id");
    query.bindValue(":specimen_name", name);
    query.bindValue(":experiment_id", experimentId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool SpecimensDAO::isSpecimenExists(int specimenId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Specimens WHERE specimen_id = :specimen_id");
    query.bindValue(":specimen_id", specimenId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}



