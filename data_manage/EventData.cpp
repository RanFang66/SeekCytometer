#include "EventData.h"
#include <QDebug>

EventData::EventData() {
    eventId = 0;
    enableSorted = false;
    isSorted = false;
}


EventData::EventData(const QVector<int> &channels)
{
    eventId = 0;
    enableSorted = false;
    isSorted = false;
    enabledChannels = channels;
    data.resize(channels.size(), QVector<int>(3, 0));
}

EventData::EventData(const QVector<int> &channels, const QByteArray &bytes)
{
    QDataStream stream(bytes);
    enabledChannels = channels;
    data.resize(channels.size(), QVector<int>(3, 0));


    quint32 header;
    quint32 magicWord;
    stream >> header;
    eventId = header & 0x00FFFFFF;
    enableSorted = ((header & 0x01000000) != 0);
    isSorted = ((header & 0x02000000) != 0);
    for (int i = 0; i < channels.size(); i++) {
        for (int j = 0; j < 3; j++) {
            stream >> data[i][j];
        }
    }
    stream >> magicWord;
}

EventData::EventData(const QVector<int> &channels, QDataStream &stream)
{
    enabledChannels = channels;
    data.resize(channels.size(), QVector<int>(3, 0));
    quint32 header;
    quint32 magicWord;
    stream >> header;
    eventId = header & 0x00FFFFFF;
    enableSorted = ((header & 0x01000000) != 0);
    isSorted = ((header & 0x02000000) != 0);
    for (int i = 0; i < channels.size(); i++) {
        for (int j = 0; j < 3; j++) {
            stream >> data[i][j];
        }
    }
    stream >> magicWord;
    if (magicWord != 0x55AA55AA) {
        qDebug() << "Magic word error!" << magicWord;
    }
}



