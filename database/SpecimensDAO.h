#ifndef SPECIMENSDAO_H
#define SPECIMENSDAO_H

#include "BaseDAO.h"
#include "Specimen.h"


// CREATE TABLE Specimens (
//     specimen_id         SERIAL PRIMARY KEY NOT NULL,
//     experiment_id       INT NOT NULL,
//     specimen_name       VARCHAR(64) NOT NULL,
//     FOREIGN KEY (experiment_id) REFERENCES Experiments(experiment_id) ON DELETE CASCADE,
//     UNIQUE (experiment_id, specimen_name)
//     );
class SpecimensDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit SpecimensDAO(QObject *parent = nullptr);

    bool insertSpecimen(const Specimen &specimen);
    bool insertSpecimen(const QString &name, int experimentId);
    bool updateSpecimen(const Specimen &specimen);
    bool deleteSpecimen(int specimenId);
    QList<Specimen> fetchSpecimens() const;
    Specimen fetchSpecimen(int specimenId) const;
    bool isSpecimenExists(const QString &name, int experimentId) const;
    bool isSpecimenExists(int specimenId) const;
};

#endif // SPECIMENSDAO_H
