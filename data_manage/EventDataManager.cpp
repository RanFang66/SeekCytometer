#include "EventDataManager.h"
#include "HistogramPlot.h"
#include "ScatterPlot.h"
#include <QFile>
#include <QDir>


EventDataManager::EventDataManager(QObject *parent)
    : QObject{parent}
{}

void EventDataManager::processHistogramData(PlotBase *plot, const QVector<EventData> &data)
{
    HistogramPlot *histogramPlot = static_cast<HistogramPlot*>(plot);
    if (!histogramPlot) return;

    int channelX = histogramPlot->axisXDetectorId();
    MeasurementType xType = histogramPlot->xMeasurementType();


    QVector<double> histogram(data.size(), 0);
    for (int i = 0; i < data.size(); ++i) {
        histogram[i] = data.at(i).getData(channelX, xType);
    }
    histogramPlot->updateData(histogram);
}

void EventDataManager::processScatterData(PlotBase *plot, const QVector<EventData> &data)
{
    ScatterPlot *scatterPlot = static_cast<ScatterPlot*>(plot);
    if (!scatterPlot) return;

    int channelX = scatterPlot->axisXDetectorId();
    MeasurementType xType = scatterPlot->xMeasurementType();
    int channelY = scatterPlot->axisYDetectorId();
    MeasurementType yType = scatterPlot->yMeasurementType();


    QVector<QPointF> scatterData(data.size());
    for (int i = 0; i < data.size(); ++i) {
        scatterData[i] = QPointF(data.at(i).getData(channelX, xType),
                                 data.at(i).getData(channelY, yType));
    }
    scatterPlot->updateData(scatterData);
}

void EventDataManager::processContourData(PlotBase *plot, const QVector<EventData> &data)
{
    // To be implemented

}

void EventDataManager::saveEventToCsvFile(const QVector<EventData> &updateData)
{
    QFile dataFile(m_dataSavePath);
    if (dataFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODeviceBase::Append)) {
        QTextStream dataStream(&dataFile);
        for (int i = 0; i < updateData.size(); i++) {
            EventData data = updateData.at(i);
            dataStream << data.getEventId();
            dataStream << ",";
            if (data.isEnabledSort()) {
                dataStream << "true";
            } else {
                dataStream << "false";
            }
            dataStream << ",";

            if (data.isRealSorted()) {
                dataStream << "true";
            } else {
                dataStream << "false";
            }
            dataStream << ",";

            for (int ch : m_enabledChannels) {
                for (MeasurementType type : MeasurementTypeHelper::measurementTypeList()) {
                    dataStream << data.getData(ch, type);
                    dataStream << ",";
                }
            }

            dataStream << "\n";
        }
        dataFile.close();
    } else {
        qDebug() << "Save Failed! Error: data file open failed!";
    }
}

void EventDataManager::initEventDataManager(const QVector<DetectorSettings> &settings)
{
    m_processedEvent = 0;
    m_enableSortEvent = 0;
    m_sortedEvent = 0;
    m_discardEvent = 0;

    m_enabledChannels.clear();
    for (const DetectorSettings &setting : settings) {
        m_enabledChannels.append(setting.detectorId());
    }
    m_eventData.init(EventData(m_enabledChannels));


    m_dataSavePath = QString("./SeekCytometerData/pulse_data_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));
    QDir saveDir(m_dataSavePath);
    if (!saveDir.exists()) {
        if (saveDir.mkpath(".")) {
            qDebug() << "Created directory " << m_dataSavePath << " ok";
        } else {
            qDebug() << "Created directory " << m_dataSavePath << " failed!";
            m_dataSavePath = ".";
        }
    }
    m_dataSavePath = saveDir.absoluteFilePath("pulsedata.csv");
    QFile dataFile(m_dataSavePath);
    if (dataFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODeviceBase::Append)) {
        QTextStream textStream(&dataFile);
        textStream << "Event ID";
        textStream << ",";
        textStream << "Sort Triggered";
        textStream << ",";
        textStream << "Sorted";
        textStream << ",";
        for (int ch : m_enabledChannels) {
            for (MeasurementType type : MeasurementTypeHelper::measurementTypeList()) {
                textStream << QString("Channel-%1(%2)").arg(ch).arg(MeasurementTypeHelper::measurementTypeToString(type));
                textStream << ",";
            }
        }
        textStream << "\n";
        dataFile.close();
    }
}

void EventDataManager::addEvent(const EventData &data)
{
    m_processedEvent++;
    if (data.isEnabledSort()) m_enableSortEvent++;
    if (data.isRealSorted()) m_sortedEvent++;
    if (data.isEnabledSort() && !data.isRealSorted()) m_discardEvent++;

    m_eventData.write(data);
}

void EventDataManager::addEvents(const QVector<EventData> &data, int enableSortNum, int sortedNum)
{
    m_processedEvent += data.size();
    m_enableSortEvent += enableSortNum;
    m_sortedEvent += sortedNum;
    m_discardEvent += (enableSortNum - sortedNum);


    m_eventData.writeMultiple(data);
    saveEventToCsvFile(data);
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



