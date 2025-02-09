#ifndef CYTOMETERSETTINGSDAO_H
#define CYTOMETERSETTINGSDAO_H

#include "BaseDAO.h"
#include "CytometerSettings.h"

// SQL for CytometerSettings table:
// CREATE TYPE ParentType AS ENUM ('Experiment', 'Specimen', 'Tube');
// CREATE TYPE ThresholdOperator AS ENUM ('OR', 'AND');

// CREATE TABLE CytometerSettings (
//     setting_id          SERIAL PRIMARY KEY NOT NULL,
//     setting_name        VARCHAR(64) NOT NULL DEFAULT 'CytometerSettings',
//     parent_type         ParentType NOT NULL,
//     experiment_id       INT REFERENCES Experiments(experiment_id) ON DELETE CASCADE,
//     specimen_id         INT REFERENCES Specimens(specimen_id) ON DELETE CASCADE,
//     tube_id             INT REFERENCES Tubes(tube_id) ON DELETE CASCADE,
//     threshold_op        ThresholdOperator NOT NULL DEFAULT 'OR',

//     -- 约束：保证一个 CytometerSettings 记录只能关联一个 parent 表
//         CHECK (
//             (parent_type = 'Experiment' AND experiment_id IS NOT NULL AND specimen_id IS NULL AND tube_id IS NULL) OR
//             (parent_type = 'Specimen' AND specimen_id IS NOT NULL AND experiment_id IS NULL AND tube_id IS NULL) OR
//             (parent_type = 'Tube' AND tube_id IS NOT NULL AND experiment_id IS NULL AND specimen_id IS NULL)
//             ),

//     UNIQUE (parent_type, experiment_id),
//     UNIQUE (parent_type, specimen_id),
//     UNIQUE (parent_type, tube_id)
//     );

class CytometerSettingsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit CytometerSettingsDAO(QObject *parent = nullptr);

    bool insertCytometerSettings(const CytometerSettings &cytometerSettings);
    bool insertCytometerSettings(const QString &name, NodeType parentType, int parentId, ThresholdType thresholdType = ThresholdType::ThresholdOr);
    bool updateCytometerSettings(const CytometerSettings &cytometerSettings);
    bool deleteCytometerSettings(int cytometerSettingsId);
    QList<CytometerSettings> fetchCytometerSettings() const;
    CytometerSettings fetchCytometerSettings(int cytometerSettingsId) const;
    bool isCytometerSettingsExists(int cytometerSettingsId) const;
    bool isCytometerSettingsExists(NodeType parentType, int parentId) const;
};

#endif // CYTOMETERSETTINGSDAO_H
