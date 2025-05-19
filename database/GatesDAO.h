#ifndef GATESDAO_H
#define GATESDAO_H

#include "BaseDAO.h"
#include "Gate.h"

// SQL FOR CREATE GATES
// CREATE TYPE GateType AS ENUM('rectangle', 'polygon', 'ellipse', 'interval', 'quadrant');
// CREATE TABLE Gates (
//     gate_id                     SERIAL PRIMARY KEY NOT NULL,
//     worksheet_id                INT NOT NULL,
//     gate_name                   VARCHAR(64) NOT NULL,
//     gate_type                   GateType NOT NULL,
//     parent_population_id        INT,
//     x_axis_id                   INT NOT NULL,
//     y_axis_id                   INT,
//     x_mearsure_type             MeasureType NOT NULL DEFAULT 'Height',
//     y_mearsure_type             MeasureType,
//     gate_data                   JSONB NOT NULL,
//     FOREIGN KEY (worksheet_id) REFERENCES WorkSheets(worksheet_id) ON DELETE CASCADE,
//     FOREIGN KEY (x_axis_id) REFERENCES DetectorSettings(detector_setting_id) ON DELETE CASCADE,
//     FOREIGN KEY (y_axis_id) REFERENCES DetectorSettings(detector_setting_id) ON DELETE CASCADE,
//     UNIQUE (worksheet_id, gate_name)
//     );


class GatesDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit GatesDAO(QObject *parent = nullptr);

    int insertGate(const Gate &gate);
    bool updateGate(const Gate &gate);
    bool deleteGate(int gateId);
    QList<Gate> fetchGates(int worksheetId) const;
    QList<Gate> fetchGates(int worksheetId, int parentPopulationId) const;
    QList<Gate> fetchGates(int worksheetId, int xAxisId, MeasurementType xMeasurementType, int yAxisId=0, MeasurementType yMeasurementType = MeasurementType::Unknown) const;
    Gate fetchGate(int gateId) const;

    bool isGateExists(int gateId) const;
    bool isGateExists(int worksheetId, const QString &name) const;
};

#endif // GATESDAO_H
