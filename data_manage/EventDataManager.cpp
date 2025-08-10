#include "EventDataManager.h"
#include "HistogramPlot.h"
#include "ScatterPlot.h"

EventDataManager::EventDataManager(QObject *parent)
    : QObject{parent}
{}

void EventDataManager::processHistogramData(PlotBase *plot, const QVector<EventData> &data)
{

}

void EventDataManager::processScatterData(PlotBase *plot, const QVector<EventData> &data)
{

}

void EventDataManager::processContourData(PlotBase *plot, const QVector<EventData> &data)
{
    // To be implemented

}

void EventDataManager::initEventDataManager(const QVector<DetectorSettings> &settings)
{
    m_enabledChannels.clear();
    for (const DetectorSettings &setting : settings) {
        m_enabledChannels.append(setting.detectorId());
    }
    m_eventData.init(EventData());
}

void EventDataManager::addEvent(const EventData &data)
{
    m_eventData.write(data);
}

void EventDataManager::addEvents(const QVector<EventData> &data)
{
    m_eventData.writeMultiple(data);
}

const QVector<EventData> &EventDataManager::getEventData()
{
    return m_eventData.readMultiple(m_eventData.avaiable());
}

void EventDataManager::processData(const QVector<PlotBase *> &plots)
{
    if (m_eventData.isEmpty()) return;
    QVector<EventData> data = m_eventData.readMultiple(m_eventData.avaiable());

    for (PlotBase *plot : plots) {
        PlotType plotType = plot->plotType();
        switch (plotType) {
        case PlotType::HISTOGRAM_PLOT:
            processHistogramData(plot, data);
            break;
        case PlotType::SCATTER_PLOT:
            processScatterData(plot, data);
            break;
        case PlotType::CONTOUR_PLOT:
            processContourData(plot, data);
            break;
        default:
            break;
        }
    }
}



