#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include "RingBuffer.h"
#include "MeasurementTypeHelper.h"
#include "PlotBase.h"
#include "DetectorSettings.h"

using SampleData = QVector<QVector<int>>;

class DataManager : public QObject
{
    Q_OBJECT
public:
    static DataManager &instance() {
        static DataManager instance;
        return instance;
    }
    DataManager &operator=(const DataManager &) = delete;
    DataManager(const DataManager &) = delete;

    void setEnabledChannels(const QVector<int> &channels) { m_enabledChannels = channels; }
    void setMeasurementTypesPerChannel(const QVector<QVector<MeasurementType>> &types) { m_measurementTypesPerChannel = types; }
    void initDataManager(const QVector<DetectorSettings> &settings);
    SampleData getEmptySampleRecord();
    int   getSampleRecordByteSize() const;



public slots:
    void addSample(const SampleData &data);
    void addSamples(const QVector<SampleData> &data);
    const QVector<SampleData> &getSampleData();
    void processData(const QVector<PlotBase*> &plots);

signals:

private:
    RingBuffer<SampleData>              m_sampleData;
    QVector<int>                        m_enabledChannels;
    QVector<QVector<MeasurementType>>   m_measurementTypesPerChannel;

    QString         m_dataSavePath;

    void processHistogramData(PlotBase *plot, const QVector<SampleData> &data);
    void processScatterData(PlotBase *plot, const QVector<SampleData> &data);
    void processContourData(PlotBase *plot, const QVector<SampleData> &data);
    void saveDataToCsvFile(const QVector<SampleData> &updateData);
    explicit DataManager(QObject *parent = nullptr);


};

#endif // DATAMANAGER_H
