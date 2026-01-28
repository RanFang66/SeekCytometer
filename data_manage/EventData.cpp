#include "EventData.h"
#include <QDebug>

EventData::EventData() {
    eventId = 0;
    enableSorted = false;
    isSorted = false;
    isValidMeasure = false;
}


EventData::EventData(const QVector<int> &channels)
{
    eventId = 0;
    enableSorted = false;
    isSorted = false;
    isValidMeasure = false;
    isValidData = false;
    diffTimeUs = 0;
    postTimeUs = 0;
    enabledChannels = channels;
    data.resize(channels.size(), QVector<int>(3, 0));

    channelIndexMap.clear();
    for (int i = 0; i < channels.size(); ++i) {
        channelIndexMap.insert(channels[i], i);
    }
}

EventData::EventData(const QVector<int> &channels, const QByteArray &bytes)
{
    QDataStream stream(bytes);
    enabledChannels = channels;
    data.resize(channels.size(), QVector<int>(3, 0));
    channelIndexMap.clear();
    for (int i = 0; i < channels.size(); ++i) {
        channelIndexMap.insert(channels[i], i);
    }

    quint32 header;
    quint32 headMagicWord;
    quint32 tailMagicWord;

    stream >> headMagicWord;
    stream >> header;
    stream >> diffTimeUs;
    stream >> postTimeUs;
    eventId = header & 0x00FFFFFF;
    enableSorted = ((header & 0x01000000) != 0);
    isSorted = ((header & 0x02000000) != 0);
    isValidMeasure = ((header & 0x04000000) != 0);
    for (int i = 0; i < channels.size(); i++) {
        for (int j = 0; j < 3; j++) {
            stream >> data[i][j];
        }
    }
    stream >> tailMagicWord;

    if (headMagicWord != HEAD_MAGIC || tailMagicWord != TAIL_MAGIC) {
        // qWarning() << "Head or Tail magic word does not match!";
        isValidData = false;
    } else {
        isValidData = true;
    }
}

EventData::EventData(const QVector<int> &channels, QDataStream &stream)
{
    enabledChannels = channels;
    data.resize(channels.size(), QVector<int>(3, 0));

    channelIndexMap.clear();
    for (int i = 0; i < channels.size(); ++i) {
        channelIndexMap.insert(channels[i], i);
    }

    quint32 header;
    quint32 headMagicWord;
    quint32 tailMagicWord;

    stream >> headMagicWord;
    stream >> header;
    eventId = header & 0x00FFFFFF;
    enableSorted = ((header & 0x01000000) != 0);
    isSorted = ((header & 0x02000000) != 0);
    isValidMeasure = ((header & 0x04000000) != 0);

    for (int i = 0; i < channels.size(); i++) {
        for (int j = 0; j < 3; j++) {
            stream >> data[i][j];
        }
    }
    stream >> tailMagicWord;
    if (headMagicWord != HEAD_MAGIC || tailMagicWord != TAIL_MAGIC) {
        // qWarning() << "Head or Tail magic word does not match!";
        isValidData = false;
    } else {
        isValidData = true;
    }
}

int EventData::getData(int channelId, MeasurementType type) const
{
    int idx = channelIndexMap.value(channelId, -1);
    if (idx < 0 || idx >= data.size()) {
        qWarning() << "getData: invalid channelId"
                   << channelId
                   << "enabledChannels =" << enabledChannels;
        return 0;
    }

    int t = static_cast<int>(type);
    if (t < 0 || t >= data[idx].size()) {
        qWarning() << "getData: invalid MeasurementType" << t;
        return 0;
    }

    return data[idx][t];
}

void EventData::setData(int channelId, MeasurementType type, int val)
{
    int idx = channelIndexMap.value(channelId, -1);
    if (idx < 0 || idx >= data.size()) {
        qWarning() << "setData: invalid channelId"
                   << channelId
                   << "enabledChannels =" << enabledChannels;
        return;
    }

    data[idx][static_cast<int>(type)] = val;
}
