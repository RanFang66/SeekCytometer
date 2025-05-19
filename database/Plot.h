#ifndef PLOT_H
#define PLOT_H

#include <QString>
#include "MeasurementTypeHelper.h"

enum class PlotType {
    HISTOGRAM_PLOT,
    SCATTER_PLOT,
    CONTOUR_PLOT,
    UNKNOWN_PLOT,
};

class Plot
{
public:
    Plot();
    static PlotType stringToPlotType(const QString &str);
    static QString plotTypeToString(PlotType type);
    static bool isValidplotType(PlotType type);
    static constexpr  int     defaultPlotSize = 512;



    int id() const { return m_id; }
    QString plotName() const {return m_plotName; }
    int workSheetId() const { return m_worksheetId; }
    PlotType plotType() const { return m_plotType; }
    int     axisXId() const { return m_axisXSettingId; }
    int     axisYId() const { return m_axisYSettingId; }
    int     axisXDetectorId() const { return m_axisXDetectorId; }
    int     axisYDetectorId() const { return m_axisYDetectorId; }
    QString axisXName() const { return m_axisXName; }
    QString axisYName() const { return m_axisYName; }
    MeasurementType xMeasurementType() const { return m_xMeasurementType; }
    MeasurementType yMeasurementType() const { return m_yMeasurementType; }


    void setId(int id) { m_id = id; }
    void setName(const QString &name) {m_plotName = name; }
    void setWorkSheetId(int id) { m_worksheetId = id; }
    void setPlotType(PlotType type) { m_plotType = type; }
    void setAxisXId(int id) { m_axisXSettingId = id; }
    void setAxisYId(int id) { m_axisYSettingId = id; }
    void setAxisXDetectorId(int id) { m_axisXDetectorId = id; }
    void setAxisYDetectorId(int id) { m_axisYDetectorId = id; }
    void setAxisXName(const QString &name) { m_axisXName = name; }
    void setAxisYName(const QString &name) { m_axisYName = name; }
    void setXMeasurementType(MeasurementType type) { m_xMeasurementType = type; }
    void setYMeasurementType(MeasurementType type) { m_yMeasurementType = type; }


private:
    int             m_id;
    QString         m_plotName;
    QString         m_axisXName;
    QString         m_axisYName;
    int             m_worksheetId;
    PlotType        m_plotType;
    int             m_axisXSettingId;
    int             m_axisYSettingId;
    int             m_axisXDetectorId;
    int             m_axisYDetectorId;
    MeasurementType m_xMeasurementType;
    MeasurementType m_yMeasurementType;
};

#endif // PLOT_H
