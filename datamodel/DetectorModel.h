#ifndef DETECTORMODEL_H
#define DETECTORMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include "Detector.h"
#include "DetectorsDAO.h"

class DetectorModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum DetectorColumn {
        DetectorIDColumn = 0,
        DetectorNameColumn,
        DetectorTypeColumn,
        FilterPeakColumn,
        FilterBandwidthColumn,
        DefaultGainColumn,
        DefaultOffsetColumn,
        ColumnCount
    };

    explicit DetectorModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void addDetector(const Detector &detector);
    void removeDetector(int row);
    void updateDetector(int row, const Detector &detector);
    const Detector &getDetector(int row) const;
    void initDetectorModel(int settingId);


private:
    const QStringList m_headerData = {"ID", "Name", "Type", "Filter Peak", "Filter Bandwidth", "Default Gain", "Default Offset"};
    DetectorsDAO detectorsDAO;
    QList<Detector> m_detectorList;
};

#endif // DETECTORMODEL_H
