#ifndef EXPERIMENTSDAO_H
#define EXPERIMENTSDAO_H

#include "BaseDAO.h"
#include "Experiment.h"

class ExperimentsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit ExperimentsDAO(QObject *parent = nullptr);

    int insertExperiment(const Experiment &experiment);
    int insertExperiment(const QString &name, int userId);
    bool updateExperiment(const Experiment &experiment);
    bool deleteExperiment(int experimentId);
    QList<Experiment> fetchExperiments() const;
    Experiment fetchExperiment(int experimentId) const;
    bool isExperimentExists(const QString &name, int userId) const;
    bool isExperimentExists(int experimentId) const;
    int  fetchExperimentId(const QString &name, int userId);
};

#endif // EXPERIMENTSDAO_H
