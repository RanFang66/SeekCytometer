#include "TreeModel.h"
#include <QModelIndex>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootNode = m_dao.getBrowserData(User::loginUser());
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentNode = nodeFromIndex(parent);
    TreeNode *childNode = parentNode->getChild(row);
    if (childNode)
        return createIndex(row, column, childNode);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TreeNode *childNode = nodeFromIndex(child);
    TreeNode *parentNode = childNode->getParent();

    if (parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeNode *parentNode = nodeFromIndex(parent);
    return parentNode->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headers.size();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeNode *node = nodeFromIndex(index);

    switch (index.column()) {
        case COLUMN_NAME:
            return node->getName();
        case COLUMN_TYPE:
            return node->getTypeString();
        case COLUMN_DATE:
            return node->getCreatedDate();
        default:
            return QVariant();
    }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);

    if (role == Qt::DisplayRole)
        return m_headers.at(section);

    return QVariant();
}

void TreeModel::addNewNode(NodeType type, TreeNode *parent)
{

}

TreeNode *TreeModel::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<TreeNode*>(index.internalPointer());
    return m_rootNode;
}


