#include "Plot.h"

Plot::Plot()
{
    m_id = -1;
    m_worksheetId = -1;
    m_axisXSettingId = -1;
    m_axisYSettingId = -1;
    m_plotType = PlotType::HISTOGRAM_PLOT;
    m_plotName = "";
    m_axisXName = "";
    m_axisYName = "";
    m_xMeasurementType = MeasurementType::Height;
    m_yMeasurementType = MeasurementType::Unknown;
}

PlotType Plot::stringToPlotType(const QString &str)
{
    if (str == "Histogram") {
        return PlotType::HISTOGRAM_PLOT;
    } else if (str == "Scatter") {
        return PlotType::SCATTER_PLOT;
    } else if (str == "Contour") {
        return PlotType::CONTOUR_PLOT;
    } else {
        return PlotType::UNKNOWN_PLOT;
    }
}

QString Plot::plotTypeToString(PlotType type)
{
    switch (type) {
    case PlotType::HISTOGRAM_PLOT:
        return "Histogram";
    case PlotType::SCATTER_PLOT:
        return "Scatter";
    case PlotType::CONTOUR_PLOT:
        return "Contour";
    default:
        return "Unknown";
    }
}

bool Plot::isValidplotType(PlotType type)
{
    return type == PlotType::HISTOGRAM_PLOT ||
            type == PlotType::SCATTER_PLOT ||
            type == PlotType::CONTOUR_PLOT;
}
