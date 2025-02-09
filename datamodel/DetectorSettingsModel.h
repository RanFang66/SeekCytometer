#ifndef DETECTORSETTINGSMODEL_H
#define DETECTORSETTINGSMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include <QVariant>
#include "DetectorSettings.h"
#include "DetectorSettingsDAO.h"

class DetectorSettingsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum DetectorSettingsColumn {
        DetectorIDColumn = 0,
        ParameterColumn,
        DetectorGainColumn,
        DetectorOffsetColumn,
        EnableHeightColumn,
        EnableWidthColumn,
        EnableAreaColumn,
        EnableThresholdColumn,
        ThresholdValueColumn,
        ColumnCount,
    };

    explicit DetectorSettingsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;


    void initDetectorModel(int configId);
    void addDetectorSettings(const DetectorSettings &detectorSettings);
    void removeDetectorSettings(int row);
    void updateDetectorSettings(int row, const DetectorSettings &detectorSettings);

    const DetectorSettings &getDetectorSettings(int row) const;

private:
    const QStringList m_headerData = {"Detector ID", "Parameter", "Gain", "Offset", "Height", "Width", "Area",
                                      "Threshold", "Threshold Value"};
    QList<DetectorSettings> m_settingsList;
    DetectorSettingsDAO m_detectorSettingsDAO;
};

#endif // DETECTORSETTINGSMODEL_H
