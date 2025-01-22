#ifndef CYTOMETERSETTINGSDAO_H
#define CYTOMETERSETTINGSDAO_H

#include "BaseDAO.h"
#include "CytometerSettings.h"
// CREATE TABLE CytometerSettings (
//     config_id       SERIAL PRIMARY KEY NOT NULL,
//     config_name     VARCHAR(64) NOT NULL DEFAULT 'CytometerSettings',
//     parent_type     ParentType NOT NULL,
//     parent_id       INT NOT NULL,
//     config_data     JSON NOT NULL,
//     threshold_op    ThresholdOperator NOT NULL DEFAULT 'OR',
//     CONSTRAINT fk_parent_experiment FOREIGN KEY (parent_id) REFERENCES Experiments(experiment_id) ON DELETE CASCADE,
//     CONSTRAINT fk_parent_specimen FOREIGN KEY (parent_id) REFERENCES Specimens(specimen_id) ON DELETE CASCADE,
//     CONSTRAINT fk_parent_tube FOREIGN KEY (parent_id) REFERENCES Tubes(tube_id) ON DELETE CASCADE,
//     UNIQUE (parent_type, parent_id)
//     );

class CytometerSettingsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit CytometerSettingsDAO(QObject *parent = nullptr);

    bool insertCytometerSettings(const CytometerSettings &cytometerSettings);
    bool updateCytometerSettings(const CytometerSettings &cytometerSettings);
    bool deleteCytometerSettings(int cytometerSettingsId);
    QList<CytometerSettings> fetchCytometerSettings() const;
    CytometerSettings fetchCytometerSettings(int cytometerSettingsId) const;
    bool isCytometerSettingsExists(int cytometerSettingsId) const;
    bool isCytometerSettingsExists(CytometerSettings::ParentType parentType, int parentId) const;
};

#endif // CYTOMETERSETTINGSDAO_H
