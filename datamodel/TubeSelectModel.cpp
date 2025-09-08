#include "TubeSelectModel.h"

TubeSelectModel::TubeSelectModel(BrowserDataModel *model, QObject *parent)
    : QAbstractTableModel{parent}, m_browserDataModel(model)
{}

int TubeSelectModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_visibleRows.size();
}

int TubeSelectModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return 1;
}

QVariant TubeSelectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    QModelIndex idx = m_visibleRows[index.row()];
    switch (role) {
    case BrowserDataModel::IsTubeRole:
        return m_browserDataModel->data(idx, BrowserDataModel::IsTubeRole);
    case BrowserDataModel::IsSelectedRole:
        return m_browserDataModel->data(idx, BrowserDataModel::IsSelectedRole);
    default:
        return QVariant();
    }

}

QVariant TubeSelectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return QString("");
        }
    }
    return QVariant();
}

bool TubeSelectModel::setData(const QModelIndex &setIndex, const QVariant &value, int role)
{
    if (!setIndex.isValid()) {
        return false;
    }
    QModelIndex idx = m_visibleRows[setIndex.row()];
    if (role == BrowserDataModel::IsSelectedRole && idx.data(BrowserDataModel::IsTubeRole).toBool()) {
        m_browserDataModel->setSelectedTube(idx.data(BrowserDataModel::NodeIdRole).toInt());
        emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
        return true;
    }

    return false;
}

void TubeSelectModel::resetTubeSelectModel(QTreeView *treeView)
{
    beginResetModel();
    m_visibleRows.clear();
    collectVisibleIndexes(QModelIndex(), m_visibleRows, treeView);
    endResetModel();
}

void TubeSelectModel::collectVisibleIndexes(const QModelIndex &parent, QList<QModelIndex> &out, QTreeView *treeView)
{
    int rows = m_browserDataModel->rowCount(parent);
    for (int r = 0; r < rows; r++) {
        QModelIndex idx = m_browserDataModel->index(r, 0, parent);
        out << idx;
        if (treeView->isExpanded(idx)) {
            collectVisibleIndexes(idx, out, treeView);
        }
    }
}
