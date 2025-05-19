#include "PlotsDAO.h"
#include <QMetaType>

PlotsDAO::PlotsDAO(QObject *parent)
    : BaseDAO{parent}
{}

int PlotsDAO::insertPlot(const Plot &plot)
{
    QSqlQuery query;
    query.prepare(R"(INSERT INTO Plots (worksheet_id, plot_type, plot_name, x_axis_id, y_axis_id, x_measure_type, y_measure_type)
                  VALUES (:worksheet_id, :plot_type, :plot_name, :x_axis_id, :y_axis_id, :x_measure_type, :y_measure_type) RETURNING plot_id)");
    query.bindValue(":worksheet_id", plot.workSheetId());
    query.bindValue(":plot_type", Plot::plotTypeToString(plot.plotType()));
    query.bindValue(":plot_name", plot.plotName());
    query.bindValue(":x_axis_id", plot.axisXId());
    query.bindValue(":y_axis_id", plot.axisYId() > 0 ? plot.axisYId() : QVariant());
    query.bindValue(":x_measure_type", MeasurementTypeHelper::measurementTypeToString(plot.xMeasurementType()));
    QVariant yMeasure =  (plot.yMeasurementType() != MeasurementType::Unknown)
                            ? (MeasurementTypeHelper::measurementTypeToString(plot.yMeasurementType())) : QVariant();
    query.bindValue(":y_measure_type", yMeasure);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        handleError(__FUNCTION__, query);
        return 0;
    }
}

int PlotsDAO::insertPlot(int worksheetId, const QString &name, PlotType type, int xAxisId, int yAxisId)
{
    Plot plot;
    plot.setWorkSheetId(worksheetId);
    plot.setPlotType(type);
    plot.setName(name);
    plot.setAxisXId(xAxisId);
    plot.setAxisYId(yAxisId);
    return insertPlot(plot);
}


bool PlotsDAO::updatePlot(const Plot &plot)
{
    QSqlQuery query;
    query.prepare("UPDATE Plots SET worksheet_id = :worksheet_id, plot_type = :plot_type, plot_name = :plot_name, "
                  "x_axis_id = :x_axis_id, y_axis_id = :y_axis_id, x_measure_type = :x_measure_type, y_measure_type = :y_measure_type WHERE plot_id = :plot_id");
    query.bindValue(":plot_id", plot.id());
    query.bindValue(":worksheet_id", plot.workSheetId());
    query.bindValue(":plot_type", Plot::plotTypeToString(plot.plotType()));
    query.bindValue(":plot_name", plot.plotName());
    query.bindValue(":x_axis_id", plot.axisXId());
    query.bindValue(":y_axis_id", plot.axisYId() > 0 ? plot.axisYId() : QVariant());
    query.bindValue(":x_measure_type", MeasurementTypeHelper::measurementTypeToString(plot.xMeasurementType()));
    query.bindValue(":y_measure_type", plot.yMeasurementType() == MeasurementType::Unknown ? QVariant() : MeasurementTypeHelper::measurementTypeToString(plot.yMeasurementType()));
    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

bool PlotsDAO::deletePlot(int plotId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Plots WHERE plot_id = :plot_id");
    query.bindValue(":plot_id", plotId);
    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

QList<Plot> PlotsDAO::fetchPlots(int worksheetId) const
{
    QList<Plot> plots;
    QSqlQuery query;
    query.prepare(R"(SELECT
        p.plot_id,
        p.plot_type,
        p.plot_name,
        p.plot_size,
        p.x_axis_id,
        dx.detector_id AS x_axis_detector_id,
        dx.parameter_name AS x_axis_parameter_name,
        p.x_measure_type,
        p.y_axis_id,
        dy.detector_id AS y_axis_detector_id,
        dy.parameter_name AS y_axis_parameter_name,
        p.y_measure_type
        FROM Plots p
        LEFT JOIN DetectorSettings dx ON p.x_axis_id = dx.detector_setting_id
        LEFT JOIN DetectorSettings dy ON p.y_axis_id = dy.detector_setting_id
        WHERE p.worksheet_id = :worksheet_id
    )");
    query.bindValue(":worksheet_id", worksheetId);
    if (query.exec()) {
        while (query.next()) {
            Plot plot;
            plot.setId(query.value("plot_id").toInt());
            plot.setWorkSheetId(worksheetId);
            plot.setPlotType(Plot::stringToPlotType(query.value("plot_type").toString()));
            plot.setName(query.value("plot_name").toString());
            plot.setAxisXId(query.value("x_axis_id").toInt());
            plot.setAxisXDetectorId(query.value("x_axis_detector_id").toInt());
            plot.setXMeasurementType(MeasurementTypeHelper::stringToMeasurementType(query.value("x_measure_type").toString()));
            plot.setAxisYId(query.value("y_axis_id").toInt());
            plot.setAxisYDetectorId(query.value("y_axis_detector_id").toInt());
            plot.setYMeasurementType(MeasurementTypeHelper::stringToMeasurementType(query.value("y_measure_type").toString()));
            QString xAxisName = MeasurementTypeHelper::parameterMeasurementType(query.value("x_axis_parameter_name").toString(), plot.xMeasurementType());
            plot.setAxisXName(xAxisName);
            if (plot.axisYId() > 0) {
                QString yAxisName = MeasurementTypeHelper::parameterMeasurementType(query.value("y_axis_parameter_name").toString(), plot.yMeasurementType());
                plot.setAxisYName(yAxisName);
            } else {
                plot.setAxisYName("Count");
            }

            plots.append(plot);
        }
    } else {
        handleError(__FUNCTION__, query);
    }

    qDebug() << QString("Fetch %1 plots under worksheet id %2").arg(plots.size()).arg(worksheetId);

    return plots;
}

Plot PlotsDAO::fetchPlot(int plotId) const
{
    Plot plot;
    QSqlQuery query;
    query.prepare(R"(SELECT
        p.plot_id,
        p.worksheet_id,
        p.plot_type,
        p.plot_name,
        p.plot_size,
        p.x_axis_id,
        dx.detector_id AS x_axis_detector_id,
        dx.parameter_name AS x_axis_parameter_name,
        p.x_measure_type,
        p.y_axis_id,
        dy.detector_id AS y_axis_detector_id,
        dy.parameter_name AS y_axis_parameter_name,
        p.y_measure_type
        FROM Plots p
        LEFT JOIN DetectorSettings dx ON p.x_axis_id = dx.detector_setting_id
        LEFT JOIN DetectorSettings dy ON p.y_axis_id = dy.detector_setting_id
        WHERE plot_id = :plot_id;
    )");
    query.bindValue(":plot_id", plotId);
    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return plot;
    }
    if (query.next()) {
        plot.setId(query.value("plot_id").toInt());
        plot.setWorkSheetId(query.value("worksheet_id").toInt());
        plot.setPlotType(Plot::stringToPlotType(query.value("plot_type").toString()));
        plot.setName(query.value("plot_name").toString());
        plot.setAxisXId(query.value("x_axis_id").toInt());
        plot.setAxisXDetectorId(query.value("x_axis_detector_id").toInt());
        plot.setXMeasurementType(MeasurementTypeHelper::stringToMeasurementType(query.value("x_measure_type").toString()));
        plot.setAxisYId(query.value("y_axis_id").toInt());
        plot.setAxisYDetectorId(query.value("y_axis_detector_id").toInt());
        plot.setYMeasurementType(MeasurementTypeHelper::stringToMeasurementType(query.value("y_measure_type").toString()));
        QString xAxisName = MeasurementTypeHelper::parameterMeasurementType(query.value("x_axis_parameter_name").toString(), plot.xMeasurementType());
        plot.setAxisXName(xAxisName);
        if (plot.axisYId() > 0) {
            QString yAxisName = MeasurementTypeHelper::parameterMeasurementType(query.value("y_axis_parameter_name").toString(), plot.yMeasurementType());
            plot.setAxisYName(yAxisName);
        } else {
            plot.setAxisYName("Count");
        }
    } else {
        qWarning() << "No plot found for plot id" << plotId;
    }
    return plot;
}


Plot PlotsDAO::fetchPlot(int worksheetId, const QString &name) const
{
    Plot plot;
    QSqlQuery query;
    query.prepare(R"(SELECT
        p.plot_id,
        p.worksheet_id,
        p.plot_type,
        p.plot_name,
        p.plot_size,
        p.x_axis_id,
        dx.detector_id AS x_axis_detector_id,
        dx.parameter_name AS x_axis_parameter_name,
        p.x_measure_type,
        p.y_axis_id,
        dy.detector_id AS y_axis_detector_id,
        dy.parameter_name AS y_axis_parameter_name,
        p.y_measure_type
        FROM Plots p
        LEFT JOIN DetectorSettings dx ON p.x_axis_id = dx.detector_setting_id
        LEFT JOIN DetectorSettings dy ON p.y_axis_id = dy.detector_setting_id
        WHERE worksheet_id = :worksheet_id AND plot_name = :plot_name;
    )");
    query.bindValue(":worksheet_id", worksheetId);
    query.bindValue(":plot_name", name);
    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return plot;
    }
    if (query.next()) {
        plot.setId(query.value("plot_id").toInt());
        plot.setWorkSheetId(query.value("worksheet_id").toInt());
        plot.setPlotType(Plot::stringToPlotType(query.value("plot_type").toString()));
        plot.setName(query.value("plot_name").toString());
        plot.setAxisXId(query.value("x_axis_id").toInt());
        plot.setAxisXDetectorId(query.value("x_axis_detector_id").toInt());
        plot.setXMeasurementType(MeasurementTypeHelper::stringToMeasurementType(query.value("x_measure_type").toString()));
        plot.setAxisYId(query.value("y_axis_id").toInt());
        plot.setAxisYDetectorId(query.value("y_axis_detector_id").toInt());
        plot.setYMeasurementType(MeasurementTypeHelper::stringToMeasurementType(query.value("y_measure_type").toString()));
        QString xAxisName = MeasurementTypeHelper::parameterMeasurementType(query.value("x_axis_parameter_name").toString(), plot.xMeasurementType());
        plot.setAxisXName(xAxisName);
        if (plot.axisYId() > 0) {
            QString yAxisName = MeasurementTypeHelper::parameterMeasurementType(query.value("y_axis_parameter_name").toString(), plot.yMeasurementType());
            plot.setAxisYName(yAxisName);
        } else {
            plot.setAxisYName("Count");
        }
    } else {
        qWarning() << "No plot found for worksheet id" << worksheetId << "and plot name" << name;
    }
    return plot;
}

bool PlotsDAO::isPlotExists(int plotId) const
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Plots WHERE plot_id = :plot_id");
    query.bindValue(":plot_id", plotId);
    return query.exec() && query.next() && query.value(0).toInt() > 0;
}

bool PlotsDAO::isPlotExists(int worksheetId, const QString &name) const
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Plots WHERE worksheet_id = :worksheet_id AND plot_name = :plot_name");
    query.bindValue(":worksheet_id", worksheetId);
    query.bindValue(":plot_name", name);
    return query.exec() && query.next() && query.value(0).toInt() > 0;
}

