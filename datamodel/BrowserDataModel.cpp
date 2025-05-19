#include "BrowserDataModel.h"
#include "BrowserDataDAO.h"
#include "ExperimentsDAO.h"
#include "SpecimensDAO.h"
#include "TubesDAO.h"
#include "CytometerSettingsDAO.h"
#include "WorkSheetsDAO.h"

BrowserDataModel::BrowserDataModel(QObject *parent)
    : QAbstractItemModel{parent}, m_rootNode(nullptr)
{
    updateDataFromDatabase();
}


QModelIndex BrowserDataModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_rootNode || !hasIndex(row, column, parent))
        return QModelIndex();

    BrowserData *parentNode = nodeFromIndex(parent);
    BrowserData *childNode = parentNode->getChild(row);
    if (childNode)
        return createIndex(row, column, childNode);
    return QModelIndex();
}

QModelIndex BrowserDataModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    BrowserData *childNode = nodeFromIndex(child);
    BrowserData *parentNode = childNode->parent();

    if (parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int BrowserDataModel::rowCount(const QModelIndex &parent) const
{
    if (!m_rootNode)
        return 0;
    BrowserData *parentNode = nodeFromIndex(parent);
    return parentNode->childCount();
}

int BrowserDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headers.size();
}

QVariant BrowserDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_rootNode)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    BrowserData *node = nodeFromIndex(index);

    switch (index.column()) {
    case COLUMN_NAME:
        return node->nodeName();
    case COLUMN_TYPE:
        return NodeTypeHelper::nodeTypeToString(node->nodeType());
    case COLUMN_DATE:
        return node->createdAt();
    default:
        return QVariant();
    }
}

Qt::ItemFlags BrowserDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant BrowserDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);

    if (role == Qt::DisplayRole)
        return m_headers.at(section);

    return QVariant();
}

BrowserData *BrowserDataModel::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<BrowserData*>(index.internalPointer());
    return m_rootNode;
}

QModelIndex BrowserDataModel::indexFromNode(BrowserData *node) const {
    if (!node || !node->parent()) {
        return QModelIndex(); // 根节点返回无效索引
    }
    int row = node->row();
    return createIndex(row, 0, node);
}

QModelIndex BrowserDataModel::findIndex(NodeType nodeType, const QString &nodeName) const
{
    return findIndexRecursive(QModelIndex(), nodeType, nodeName);
}


QModelIndex BrowserDataModel::findIndexRecursive(const QModelIndex &parent, NodeType nodeType, const QString &nodeName) const
{
    int rowCount = this->rowCount(parent);
    for (int i = 0; i < rowCount; ++i) {
        QModelIndex index = this->index(i, 0, parent);
        BrowserData *node = nodeFromIndex(index);

        if (node && node->nodeType() == nodeType && node->nodeName() == nodeName) {
            return index; // 找到匹配的节点，返回其 QModelIndex
        }

        // 递归搜索子节点
        QModelIndex childIndex = findIndexRecursive(index, nodeType, nodeName);
        if (childIndex.isValid()) {
            return childIndex;
        }
    }
    return QModelIndex(); // 未找到，返回无效索引
}

QModelIndex BrowserDataModel::getAncestorIndex(const QModelIndex &current, NodeType ancestorNodeType) const
{
    if (!current.isValid() || !nodeFromIndex(current)
        || ancestorNodeType == NodeType::Settings || ancestorNodeType == NodeType::Unknown) {
        return QModelIndex();
    }

    int  depthDiff = nodeFromIndex(current)->depth() - NodeTypeHelper::nodeTypeDepth(ancestorNodeType);
    if (depthDiff < 0) {
        return QModelIndex();
    }

    QModelIndex ancestorModelIndex = current;
    while (depthDiff > 0) {
        ancestorModelIndex = ancestorModelIndex.parent();
        depthDiff--;
    }
    return ancestorModelIndex;
}


bool BrowserDataModel::updateDataFromDatabase()
{
    beginResetModel();
    deleteTree(m_rootNode);
    m_rootNode = nullptr;
    BrowserDataDAO dao;
    m_rootNode = dao.fetchBrowserData(User::loginUser());
    endResetModel();

    return (m_rootNode != nullptr);
}

QModelIndex BrowserDataModel::updateNewNode(NodeType nodeType, const QModelIndex &parent, int nodeId)
{
    if (!parent.isValid()) {
        return QModelIndex();
    }
    BrowserData *parentNode = nodeFromIndex(parent);
    if (!parentNode) {
        return QModelIndex();
    }

    BrowserData *newNode = BrowserDataDAO().fetechNode(parentNode, nodeType, nodeId);
    if (!newNode) {
        return QModelIndex();
    }

    beginInsertRows(parent, parentNode->childCount(), parentNode->childCount());
    parentNode->addChild(newNode);
    endInsertRows();
    return indexFromNode(newNode);
}

bool BrowserDataModel::insertNewNode(NodeType nodeType, const QString &nodeName, const QModelIndex &parent)
{
    if (!parent.isValid()) {
        return false;
    }

    BrowserData *parentNode = nodeFromIndex(parent);
    if (!parentNode) {
        return false;
    }

    int nodeId = 0;
    switch (nodeType) {
        case NodeType::Experiment:
            nodeId = ExperimentsDAO().insertExperiment(nodeName, parentNode->nodeId());
            break;
        case NodeType::Specimen:
            nodeId = SpecimensDAO().insertSpecimen(nodeName, parentNode->nodeId());
            break;
        case NodeType::Tube:
            nodeId = TubesDAO().insertTube(nodeName, parentNode->nodeId());
            break;
        case NodeType::Settings:
            nodeId = CytometerSettingsDAO().insertCytometerSettings(nodeName, parentNode->nodeType(), parentNode->nodeId());
            break;
        case NodeType::Worksheet:
            nodeId = WorkSheetsDAO().insertWorkSheet(nodeName, (parentNode->nodeType() == NodeType::Experiment), parentNode->nodeId());
            break;
        default:
            break;
    }

    if (nodeId <= 0) {
        return false;
    }

    BrowserData *newNode = BrowserDataDAO().fetechNode(parentNode, nodeType, nodeId);
    if (!newNode) {
        return false;
    }
    beginInsertRows(parent, parentNode->childCount(), parentNode->childCount());
    parentNode->addChild(newNode);
    endInsertRows();
    return true;
}


QList<QModelIndex> BrowserDataModel::getNodePath(const QModelIndex &index)
{
    QList<QModelIndex> path;
    QModelIndex currentIndex = index;

    while (currentIndex.isValid()) {
        path.prepend(currentIndex);
        currentIndex = currentIndex.parent();
    }
    return path;
}


void BrowserDataModel::deleteTree(BrowserData *node)
{
    if (!node) return;
    for (BrowserData *child : node->children()) {
        deleteTree(child);
    }
    delete node;
}
