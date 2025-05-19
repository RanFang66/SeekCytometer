#ifndef PLOTSDAO_H
#define PLOTSDAO_H

#include "BaseDAO.h"
#include "Plot.h"

// SQL FOR CREATE PLOTS
// CREATE TYPE PlotType AS ENUM('histogram', 'scatter', 'contour');
// CREATE TYPE MeasureType AS ENUM('Height', 'Width', 'Area');
// CREATE TABLE Plots (
//     plot_id         SERIAL PRIMARY KEY NOT NULL,
//     worksheet_id    INT NOT NULL,
//     plot_type       PlotType NOT NULL,
//     plot_name       VARCHAR(64) NOT NULL,
//     x_axis_id INT NOT NULL,
//     y_axis_id INT,
//     x_mearsure_type   MeasureType NOT NULL DEFAULT 'Height',
//     y_mearsure_type   MeasureType,
//     plot_size       INT NOT NULL DEFAULT 512,
//     FOREIGN KEY (worksheet_id) REFERENCES WorkSheets(worksheet_id) ON DELETE CASCADE,
//     FOREIGN KEY (x_axis_id) REFERENCES DetectorSettings(detector_setting_id) ON DELETE CASCADE,
//     FOREIGN KEY (y_axis_id) REFERENCES DetectorSettings(detector_setting_id) ON DELETE CASCADE,
//     UNIQUE (worksheet_id, plot_name)
//     );


class PlotsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit PlotsDAO(QObject *parent = nullptr);
    int insertPlot(const Plot &plot);
    int insertPlot(int worksheetId, const QString &name, PlotType type, int xAxisId, int yAxisId = 0);
    bool updatePlot(const Plot &plot);
    bool deletePlot(int plotId);
    QList<Plot> fetchPlots(int worksheetId) const;
    Plot fetchPlot(int plotId) const;
    Plot fetchPlot(int worksheetId, const QString &name) const;
    bool isPlotExists(int plotId) const;
    bool isPlotExists(int worksheetId, const QString &name) const;
};

#endif // PLOTSDAO_H
