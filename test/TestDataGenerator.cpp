#include "TestDataGenerator.h"
#include <QRandomGenerator>

TestDataGenerator::TestDataGenerator(QObject *parent)
    : QObject{parent}, m_generateTimer(new QTimer(this)), m_dataCount(100), m_interval(500), m_dataMin(0), m_dataMax(32768)
{
    m_generateTimer->setSingleShot(false);
    m_generateTimer->stop();
    connect(m_generateTimer, &QTimer::timeout, this, &TestDataGenerator::generateTestData);
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


