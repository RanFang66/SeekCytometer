#include "ExperimentsDAO.h"

ExperimentsDAO::ExperimentsDAO(QObject *parent)
    : BaseDAO{parent}
{}

bool ExperimentsDAO::insertExperiment(const Experiment &experiment)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Experiments (user_id, experiment_name) VALUES (:user_id, :experiment_name)");
    query.bindValue(":user_id", experiment.userId());
    query.bindValue(":experiment_name", experiment.name());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool ExperimentsDAO::updateExperiment(const Experiment &experiment)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Experiments SET experiment_name = :experiment_name WHERE experiment_id = :experiment_id");
    query.bindValue(":experiment_name", experiment.name());
    query.bindValue(":experiment_id", experiment.id());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool ExperimentsDAO::deleteExperiment(int experimentId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Experiments WHERE experiment_id = :experiment_id");
    query.bindValue(":experiment_id", experimentId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


QList<Experiment> ExperimentsDAO::fetchExperiments() const
{
    QList<Experiment> experiments;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Experiments");

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return experiments;
    }

    while (query.next()) {
        Experiment experiment;
        experiment.setId(query.value("experiment_id").toInt());
        experiment.setUserId(query.value("user_id").toInt());
        experiment.setName(query.value("experiment_name").toString());
        experiments.append(experiment);
    }

    return experiments;
}


Experiment ExperimentsDAO::fetchExperiment(int experimentId) const
{
    Experiment experiment;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Experiments WHERE experiment_id = :experiment_id");
    query.bindValue(":experiment_id", experimentId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return experiment;
    }

    if (query.next()) {
        experiment.setId(query.value("experiment_id").toInt());
        experiment.setUserId(query.value("user_id").toInt());
        experiment.setName(query.value("experiment_name").toString());
    }

    return experiment;
}


bool ExperimentsDAO::isExperimentExists(const QString &name) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT experiment_id FROM Experiments WHERE experiment_name = :experiment_name");
    query.bindValue(":experiment_name", name);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool ExperimentsDAO::isExperimentExists(int experimentId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT experiment_id FROM Experiments WHERE experiment_id = :experiment_id");
    query.bindValue(":experiment_id", experimentId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}



