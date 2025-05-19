#include "DetectorModel.h"


DetectorModel::DetectorModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int DetectorModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_detectorList.size();
}

int DetectorModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return DetectorColumn::ColumnCount;
}

QVariant DetectorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const Detector &detector = m_detectorList.at(index.row());
        switch (index.column()) {
        case DetectorColumn::DetectorIDColumn:
            return detector.id();
        case DetectorColumn::DetectorNameColumn:
            return detector.name();
        case DetectorColumn::DetectorTypeColumn:
            return detector.type();
        case DetectorColumn::FilterPeakColumn:
            return detector.filterPeak();
        case DetectorColumn::FilterBandwidthColumn:
            return detector.filterBandwidth();
        case DetectorColumn::DefaultGainColumn:
            return detector.defaultGain();
        case DetectorColumn::DefaultOffsetColumn:
            return detector.defaultOffset();
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QVariant DetectorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return m_headerData.at(section);
        }
    }
    return QVariant();
}

bool DetectorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (role == Qt::EditRole) {
        Detector &detector = m_detectorList[index.row()];
        switch (index.column()) {
        case DetectorColumn::DetectorNameColumn:
            detector.setName(value.toString());
            break;
        case DetectorColumn::DetectorTypeColumn:
            detector.setType(value.toString());
            break;
        case DetectorColumn::FilterPeakColumn:
            detector.setFilterPeak(value.toInt());
            break;
        case DetectorColumn::FilterBandwidthColumn:
            detector.setFilterBandwidth(value.toInt());
            break;
        case DetectorColumn::DefaultGainColumn:
            detector.setDefaultGain(value.toInt());
            break;
        case DetectorColumn::DefaultOffsetColumn:
            detector.setDefaultOffset(value.toInt());
            break;
        default:
            return false;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void DetectorModel::addDetector(const Detector &detector)
{
    beginInsertRows(QModelIndex(), m_detectorList.size(), m_detectorList.size());
    m_detectorList.append(detector);
    endInsertRows();
}

void DetectorModel::removeDetector(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_detectorList.removeAt(row);
    endRemoveRows();
}

void DetectorModel::updateDetector(int row, const Detector &detector)
{
    m_detectorList[row] = detector;
    emit dataChanged(index(row, 0), index(row, DetectorColumn::ColumnCount));
}

const Detector &DetectorModel::getDetector(int row) const
{
    return m_detectorList.at(row);
}

void DetectorModel::initDetectorModel(int settingId)
{
    beginResetModel();
    m_detectorList = detectorsDAO.fetchDetectorsUnset(settingId);
    endResetModel();
}





