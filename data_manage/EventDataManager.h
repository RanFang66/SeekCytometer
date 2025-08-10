#ifndef EVENTDATAMANAGER_H
#define EVENTDATAMANAGER_H

#include <QObject>
#include "RingBuffer.h"

#include "MeasurementTypeHelper.h"

#include "PlotBase.h"
#include "DetectorSettings.h"




class EventData {

public:
    EventData()
    {
        eventId = 0;
        channelMask = 0;
        for (int i = 0; i < 8; i++) {
            peak[i] = 0;
            width[i] = 0;
            area[i] = 0;
        }
    }



    quint32 eventId;
    quint8  channelMask;
    qint32  peak[8];
    quint32 width[8];
    qint32  area[8];
};



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

public slots:
    void addEvent(const EventData &data);
    void addEvents(const QVector<EventData> &data);
    const QVector<EventData> &getEventData();

    void processData(const QVector<PlotBase*> &plots);

signals:


private:
    explicit EventDataManager(QObject *parent = nullptr);

    void processHistogramData(PlotBase *plot, const QVector<EventData> &data);
    void processScatterData(PlotBase *plot, const QVector<EventData> &data);
    void processContourData(PlotBase *plot, const QVector<EventData> &data);


    RingBuffer<EventData>               m_eventData;

    QVector<int>                        m_enabledChannels;

};

#endif // EVENTDATAMANAGER_H
