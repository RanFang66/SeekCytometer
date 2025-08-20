#include "TestDataGenerator.h"
#include <QRandomGenerator>
#include <EventDataManager.h>
TestDataGenerator::TestDataGenerator(QObject *parent)
    : QObject{parent}, m_generateTimer(new QTimer(this)), m_dataCount(100), m_interval(500), m_dataMin(0), m_dataMax(32768)
{
    m_eventId = 0;
    m_generateTimer->setSingleShot(false);
    m_generateTimer->stop();
    // connect(m_generateTimer, &QTimer::timeout, this, &TestDataGenerator::generateTestData);
    connect(m_generateTimer, &QTimer::timeout, this, &TestDataGenerator::generateEventData);
}

void TestDataGenerator::configTestGenerator(int dataCount, int interval, int dataMin, int dataMax)
{
    QMutexLocker locker(&m_mutex);
    m_dataCount = dataCount;
    m_interval = interval;
    m_dataMin = dataMin;
    m_dataMax = dataMax;
}


void TestDataGenerator::startGenerateData()
{
    m_eventId = 0;
    m_generateTimer->start(m_interval);
}

void TestDataGenerator::generateTestData()
{
    QVector<SampleData> generatedData;
    for (int i = 0; i < m_dataCount; i++) {
        SampleData data = DataManager::instance().getEmptySampleRecord();
        for (int chIndex = 0; chIndex < data.size(); ++chIndex) {
            for (int mIndex = 0; mIndex < data.at(chIndex).size(); ++mIndex) {
                data[chIndex][mIndex] = QRandomGenerator::global()->bounded(m_dataMin, m_dataMax);
            }
        }
        generatedData.append(data);
    }
    emit testDataGenerated(generatedData);
}

void TestDataGenerator::generateEventData()
{
    QVector<EventData> eventData;
    int enableSortNum = 0;
    int sortedNum = 0;
    for (int count = 0; count < m_dataCount; count++) {
        m_eventId++;
        EventData event(EventDataManager::instance().enabledChannels());
        event.setEventId(m_eventId);
        int val = QRandomGenerator::global()->bounded(0, 100);
        if (val > 30) {
            event.setEnableSort(true);
            enableSortNum++;
            if (val > 45) {
                event.setSorted(true);
                sortedNum++;
            }
        }

        for (int ch : EventDataManager::instance().enabledChannels()) {
            for (MeasurementType type : MeasurementTypeHelper::measurementTypeList()) {
                event.setData(ch, type, QRandomGenerator::global()->bounded(m_dataMin, m_dataMax));
            }
        }
        eventData.append(event);
    }

    emit eventDataGenerated(eventData, enableSortNum, sortedNum);
}

void TestDataGenerator::stopGenerateData()
{
    m_generateTimer->stop();
}

void TestDataGenerator::resetGenerator()
{
    QMutexLocker locker(&m_mutex);
    m_dataCount = 100;
    m_interval = 500;
    m_dataMin = 0;
    m_dataMax = 32768;
}


