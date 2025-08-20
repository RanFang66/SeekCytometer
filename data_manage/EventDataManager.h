#ifndef EVENTDATAMANAGER_H
#define EVENTDATAMANAGER_H

#include <QObject>
#include "RingBuffer.h"

#include "MeasurementTypeHelper.h"

#include "PlotBase.h"
#include "DetectorSettings.h"
#include "EventData.h"



class EventDataManager : public QObject
{
    Q_OBJECT
public:
    static EventDataManager &instance() {
        static EventDataManager instance;
        return instance;
    }
    EventDataManager &operator=(const EventDataManager &) = delete;
    EventDataManager(const EventDataManager &) = delete;

    void initEventDataManager(const QVector<DetectorSettings> &settings);
    const QVector<int> &enabledChannels() const;
    int sortedEventNum() const;
    int enableSortedEventNum() const;
    int processedEventNum() const;
    int discardedEventNum() const;


public slots:
    void addEvent(const EventData &data);
    void addEvents(const QVector<EventData> &data, int enableSortNum, int sortedNum);
    const QVector<EventData> &getEventData();

    void processData(const QVector<PlotBase*> &plots);

signals:


private:
    explicit EventDataManager(QObject *parent = nullptr);

    void processHistogramData(PlotBase *plot, const QVector<EventData> &data);
    void processScatterData(PlotBase *plot, const QVector<EventData> &data);
    void processContourData(PlotBase *plot, const QVector<EventData> &data);

    void saveEventToCsvFile(const QVector<EventData> &updateData);


    QString         m_dataSavePath;

    RingBuffer<EventData>               m_eventData;

    int             m_processedEvent;
    int             m_enableSortEvent;
    int             m_sortedEvent;
    int             m_discardEvent;


    QVector<int>                        m_enabledChannels;

};

inline const QVector<int> &EventDataManager::enabledChannels() const
{
    return m_enabledChannels;
}

inline int EventDataManager::sortedEventNum() const
{
    return m_sortedEvent;
}

inline int EventDataManager::enableSortedEventNum() const
{
    return m_enableSortEvent;
}

inline int EventDataManager::processedEventNum() const
{
    return m_processedEvent;
}

inline int EventDataManager::discardedEventNum() const
{
    return m_discardEvent;
}



#endif // EVENTDATAMANAGER_H
