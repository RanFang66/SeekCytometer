#include "TubesDAO.h"

TubesDAO::TubesDAO(QObject *parent)
    : BaseDAO{parent}
{}


// CREATE TABLE Tubes (
//     tube_id             SERIAL PRIMARY KEY NOT NULL,
//     specimen_id         INT NOT NULL,
//     tube_name           VARCHAR(64) NOT NULL,
//     FOREIGN KEY (specimen_id) REFERENCES Specimens(specimen_id) ON DELETE CASCADE,
//     UNIQUE (specimen_id, tube_name)
//     );
bool TubesDAO::insertTube(const Tube &tube)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Tubes (specimen_id, tube_name) VALUES (:specimen_id, :tube_name)");
    query.bindValue(":specimen_id", tube.specimenId());
    query.bindValue(":tube_name", tube.name());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

bool TubesDAO::updateTube(const Tube &tube)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Tubes SET tube_name = :tube_name WHERE tube_id = :tube_id");
    query.bindValue(":tube_name", tube.name());
    query.bindValue(":tube_id", tube.id());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool TubesDAO::deleteTube(int tubeId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Tubes WHERE tube_id = :tube_id");
    query.bindValue(":tube_id", tubeId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

QList<Tube> TubesDAO::fetchTubes() const
{
    QList<Tube> tubes;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Tubes");

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return tubes;
    }

    while (query.next()) {
        Tube tube;
        tube.setId(query.value("tube_id").toInt());
        tube.setSpecimenId(query.value("specimen_id").toInt());
        tube.setName(query.value("tube_name").toString());
        tubes.append(tube);
    }

    return tubes;
}


Tube TubesDAO::fetchTube(int tubeId) const
{
    Tube tube;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Tubes WHERE tube_id = :tube_id");
    query.bindValue(":tube_id", tubeId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return tube;
    }

    if (query.next()) {
        tube.setId(query.value("tube_id").toInt());
        tube.setSpecimenId(query.value("specimen_id").toInt());
        tube.setName(query.value("tube_name").toString());
    }

    return tube;
}

bool TubesDAO::isTubeExists(const QString &name) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Tubes WHERE tube_name = :tube_name");
    query.bindValue(":tube_name", name);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool TubesDAO::isTubeExists(int tubeId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Tubes WHERE tube_id = :tube_id");
    query.bindValue(":tube_id", tubeId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}



