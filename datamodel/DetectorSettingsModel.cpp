#include "DetectorSettingsModel.h"

DetectorSettingsModel::DetectorSettingsModel(QObject *parent)
    : QAbstractTableModel{parent}
{}


int DetectorSettingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_settingsList.size();
}

int DetectorSettingsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headerData.size();
}

QVariant DetectorSettingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || index.column() >= columnCount()) {
        return QVariant();
    }

    const auto &settings = m_settingsList[index.row()];
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case DetectorIDColumn:      return settings.detectorId();
        case ParameterColumn:       return settings.parameter();
        case DetectorGainColumn:    return settings.detectorGain();
        case DetectorOffsetColumn:  return settings.detectorOffset();
        case EnableHeightColumn:    return settings.isEnabledHeight();
        case EnableWidthColumn:     return settings.isEnabledWidth();
        case EnableAreaColumn:      return settings.isEenabledArea();
        case EnableThresholdColumn: return settings.isEnabledThreshold();
        case ThresholdValueColumn:  return settings.thresholdValue();
        default: return QVariant();
        }
    } else if (role == Qt::CheckStateRole && index.column() == EnableHeightColumn) {
        return (settings.isEnabledHeight() ? Qt::Checked : Qt::Unchecked);
    } else if (role == Qt::CheckStateRole && index.column() == EnableWidthColumn) {
        return (settings.isEnabledWidth() ? Qt::Checked : Qt::Unchecked);
    } else if (role == Qt::CheckStateRole && index.column() == EnableAreaColumn) {
        return (settings.isEenabledArea() ? Qt::Checked : Qt::Unchecked);
    } else if (role == Qt::CheckStateRole && index.column() == EnableThresholdColumn) {
        return (settings.isEnabledThreshold() ? Qt::Checked : Qt::Unchecked);
    }

    return QVariant();
}


QVariant DetectorSettingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal && section >= 0 && section < m_headerData.length()) {
            return m_headerData.at(section);
        } else if (orientation == Qt::Vertical && section >=0 && section < m_settingsList.length()) {
            return tr("Detector-")+ QString::number(section);
        }
    }
    return QVariant();
}

bool DetectorSettingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= rowCount() || index.column() >= columnCount()) {
        return false;
    }

    auto &settings = m_settingsList[index.row()];
    if (role == Qt::EditRole) {
        switch (index.column()) {
        case DetectorIDColumn:      return false;
        case ParameterColumn:       settings.setParameter(value.toString()); break;
        case DetectorGainColumn:    settings.setDetectorGain(value.toInt()); break;
        case DetectorOffsetColumn:  settings.setDetectorOffset(value.toInt()); break;
        case EnableHeightColumn:    settings.enableHeight(value.toBool()); break;
        case EnableWidthColumn:     settings.enableWidth(value.toBool()); break;
        case EnableAreaColumn:      settings.enableArea(value.toBool()); break;
        case EnableThresholdColumn: settings.enableThreshold(value.toBool()); break;
        case ThresholdValueColumn:  settings.setThresholdValue(value.toInt()); break;
        default: return false;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void DetectorSettingsModel::initDetectorModel(int configId)
{
    beginResetModel();
    m_settingsList = m_detectorSettingsDAO.fetchDetectorSettings(configId);
    endResetModel();
}

void DetectorSettingsModel::addDetectorSettings(const DetectorSettings &detectorSettings)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_settingsList.append(detectorSettings);
    endInsertRows();
}

void DetectorSettingsModel::removeDetectorSettings(int row)
{
    if (row < 0 || row >= rowCount()) {
        return;
    }
    beginRemoveRows(QModelIndex(), row, row);
    m_settingsList.removeAt(row);
    endRemoveRows();
}

void DetectorSettingsModel::updateDetectorSettings(int row, const DetectorSettings &detectorSettings)
{
    if (row < 0 || row >= rowCount()) {
        return;
    }
    m_settingsList[row] = detectorSettings;
    emit dataChanged(index(row, 0), index(row, columnCount()));
}

const DetectorSettings &DetectorSettingsModel::getDetectorSettings(int row) const
{
    return m_settingsList[row];
}
