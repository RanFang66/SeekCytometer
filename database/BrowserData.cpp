#include "BrowserData.h"

BrowserData::BrowserData() {}

BrowserData::BrowserData(int id, int parentId, const QString &name, NodeType type, int nodeId, int depth,
                         const QDateTime &createdAt, const QDateTime &updatedAt, BrowserData *parent)
    : m_id(id), m_parentId(parentId), m_nodeName(name), m_nodeType(type), m_nodeId(nodeId),
    m_depth(depth), m_createdAt(createdAt), m_updatedAt(updatedAt), m_parent(parent)
{
}

QList<BrowserData *> BrowserData::getNodePath() const
{
    QList<BrowserData*> nodePath;
    const BrowserData *current = this;

    while(current && current->parent()->nodeType() != NodeType::Root) {
        nodePath.prepend(current->parent());
        current = current->parent();
    }

    return nodePath;
}
