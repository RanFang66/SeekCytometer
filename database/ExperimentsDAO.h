#ifndef EXPERIMENTSDAO_H
#define EXPERIMENTSDAO_H

#include "BaseDAO.h"
#include "Experiment.h"

class ExperimentsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit ExperimentsDAO(QObject *parent = nullptr);

    bool insertExperiment(const Experiment &experiment);
    bool updateExperiment(const Experiment &experiment);
    bool deleteExperiment(int experimentId);
    QList<Experiment> fetchExperiments() const;
    Experiment fetchExperiment(int experimentId) const;
    bool isExperimentExists(const QString &name) const;
    bool isExperimentExists(int experimentId) const;
};

#endif // EXPERIMENTSDAO_H
