#ifndef DETECTORSETTINGSMODEL_H
#define DETECTORSETTINGSMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include <QVariant>
#include "DetectorSettings.h"
#include <QMutex>

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

    static DetectorSettingsModel *instance() {
        static DetectorSettingsModel instance;
        return &instance;
    }
    DetectorSettingsModel(const DetectorSettingsModel &) = delete;
    DetectorSettingsModel &operator=(const DetectorSettingsModel &) = delete;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;


    void resetDetectorSettingModel(int settingId);
    void addDetectorSettings(const DetectorSettings &detectorSettings);
    void removeDetectorSettings(int row);
    void updateDetectorSettings(int row, const DetectorSettings &detectorSettings);

    int getSettingId() const;
    const DetectorSettings &getDetectorSettings(int row) const;
    const QList<DetectorSettings> &detectorSettings() const;

private:
    explicit DetectorSettingsModel(QObject *parent = nullptr);
    const QStringList m_headerData = {"Detector ID", "Parameter", "Gain", "Offset", "Height", "Width", "Area",
                                      "Threshold", "Threshold Value"};
    QList<DetectorSettings> m_settingsList;
    QMutex m_mutex;
    int m_settingId;
};

inline int DetectorSettingsModel::getSettingId() const
{
    return m_settingId;
}

#endif // DETECTORSETTINGSMODEL_H
