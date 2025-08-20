#ifndef TESTDATAGENERATOR_H
#define TESTDATAGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QMutex>

#include "DataManager.h"
#include "EventData.h"
// class DetectorData
// {
// public:
//     explicit DetectorData();
//     DetectorData(int id, bool enHeight, bool enWidth, bool enArea) : m_id(id), m_enableHeight(enHeight), m_enableWidth(enWidth), m_enableArea(enArea) {}
//     void configDetectorData(int id, bool enableHeight, bool enableWidth, bool enableArea) { m_id = id; m_enableHeight = enableHeight; m_enableWidth = enableWidth; m_enableArea = enableArea; }
//     void appendData(int height, int width, int area) {  m_height.append(height); m_width.append(width); m_area.append(area); }
//     void appendHeight(int height) {m_height.append(height);}
//     void appendWidth(int width) {m_width.append(width);}
//     void appendArea(int area) {m_area.append(area);}
//     void clearData() { m_height.clear(); m_width.clear(); m_area.clear(); }

//     int  id() const { return m_id; }
//     bool enableHeight() const { return m_enableHeight; }
//     bool enableWidth() const { return m_enableWidth; }
//     bool enableArea() const { return m_enableArea; }
//     const QVector<int> &height() const { return m_height; }
//     const QVector<int> &width() const { return m_width; }
//     const QVector<int> &area() const { return m_area; }


// private:
//     int  m_id;
//     bool m_enableHeight;
//     bool m_enableWidth;
//     bool m_enableArea;
//     QVector<int> m_height;
//     QVector<int> m_width;
//     QVector<int> m_area;
// };



class TestDataGenerator : public QObject
{
    Q_OBJECT
public:
    static TestDataGenerator &instance() {
        static TestDataGenerator instance;
        return instance;
    }

    TestDataGenerator(const TestDataGenerator &) = delete;
    TestDataGenerator &operator=(const TestDataGenerator &) = delete;



    void configTestGenerator(int dataCount, int interval, int dataMin, int dataMax);
    void startGenerateData();
    void stopGenerateData();
    void resetGenerator();


signals:
    void testDataGenerated(const QVector<SampleData> &generatedData);
    void eventDataGenerated(const QVector<EventData> &eventData, int enableSortNum, int sortedNum);

private slots:
    void generateTestData();
    void generateEventData();

private:
    QTimer                  *m_generateTimer;
    int                     m_dataCount;
    int                     m_interval;
    int                     m_dataMin;
    int                     m_dataMax;
    int                     m_eventId;

    QMutex                  m_mutex;

    explicit TestDataGenerator(QObject *parent = nullptr);

};

#endif // TESTDATAGENERATOR_H
