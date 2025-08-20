#ifndef EVENTDATA_H
#define EVENTDATA_H


#include <QObject>
#include <QVector>
#include "MeasurementTypeHelper.h"

class EventData
{
public:
    EventData();
    explicit EventData(const QVector<int> &enabledChannels);
    explicit EventData(const QVector<int> &enabledChannels, const QByteArray &bytes);
    explicit EventData(const QVector<int> &enabledChannels, QDataStream &stream);
    int getWidth(int channelId) const;
    int getHeight(int channelId) const;
    int getArea(int channelId) const;
    int getData(int channelId, MeasurementType type) const;
    bool isEnabledSort() const;
    bool isRealSorted() const;
    int getEventId() const;

    void setEventId(int id);
    void setData(int channelId, MeasurementType type, int val);
    void setEnableSort(bool enabled);
    void setSorted(bool enabled);

private:
    quint64                 eventId;
    bool                    enableSorted;
    bool                    isSorted;
    QVector<int>            enabledChannels;
    QVector<QVector<int>>   data;
};

inline int EventData::getHeight(int channelId) const
{
    return data[enabledChannels.indexOf(channelId)].at(static_cast<int>(MeasurementType::Height));
}


inline int EventData::getWidth(int channelId) const
{
    return data[enabledChannels.indexOf(channelId)].at(static_cast<int>(MeasurementType::Width));
}


inline int EventData::getArea(int channelId) const
{
    return data[enabledChannels.indexOf(channelId)].at(static_cast<int>(MeasurementType::Area));
}

inline int EventData::getData(int channelId, MeasurementType type) const
{
    return data[enabledChannels.indexOf(channelId)].at(static_cast<int>(type));
}

inline bool EventData::isEnabledSort() const
{
    return enableSorted;
}

inline bool EventData::isRealSorted() const
{
    return isSorted;
}

inline int EventData::getEventId() const
{
    return eventId;
}

inline void EventData::setEventId(int id)
{
    eventId = id;
}

inline void EventData::setData(int channelId, MeasurementType type, int val)
{
    data[enabledChannels.indexOf(channelId)][static_cast<int>(type)] = val;
}

inline void EventData::setEnableSort(bool enabled)
{
    enableSorted = enabled;
}

inline void EventData::setSorted(bool enabled)
{
    isSorted = enabled;
}



#endif // EVENTDATA_H
