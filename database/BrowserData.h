#ifndef BROWSERDATA_H
#define BROWSERDATA_H
#include <QString>
#include <QList>
#include <QDateTime>
#include "NodeTypeHelper.h"

class BrowserData
{
public:
    BrowserData();
    BrowserData(int id, int parentId, const QString &name, NodeType type, int nodeId, int depth,
                const QDateTime &createdAt=QDateTime::currentDateTime(),
                const QDateTime &updatedAt=QDateTime::currentDateTime(),
                BrowserData *parent = nullptr);

    int id() const;
    int parentId() const;
    QString nodeName() const;
    NodeType nodeType() const;
    int nodeId() const;
    int depth() const;
    int childCount() const;
    QDateTime createdAt() const;
    QDateTime updatedAt() const;
    BrowserData *parent() const;
    int row() const;
    BrowserData *getChild(int row) const;
    const QList<BrowserData*> &children() const;
    void addChild(BrowserData *child);

    QList<BrowserData*> getNodePath() const;

private:
    int m_id;
    int m_parentId;
    QString m_nodeName;
    NodeType m_nodeType;
    int m_nodeId;
    int m_depth;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
    QList<BrowserData *> m_children;
    BrowserData *m_parent;
};

inline int BrowserData::id() const
{
    return m_id;
}

inline int BrowserData::parentId() const
{
    return m_parentId;
}

inline QString BrowserData::nodeName() const
{
    return m_nodeName;
}

inline NodeType BrowserData::nodeType() const
{
    return m_nodeType;
}

inline int BrowserData::nodeId() const
{
    return m_nodeId;
}

inline int BrowserData::depth() const
{
    return m_depth;
}

inline int BrowserData::childCount() const
{
    return m_children.size();
}

inline QDateTime BrowserData::createdAt() const
{
    return m_createdAt;
}

inline QDateTime BrowserData::updatedAt() const
{
    return m_updatedAt;
}

inline BrowserData *BrowserData::parent() const
{
    return m_parent;
}

inline int BrowserData::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<BrowserData*>(this));
    return 0;
}

inline BrowserData *BrowserData::getChild(int row) const
{
    return m_children.value(row);
}

inline const QList<BrowserData *> &BrowserData::children() const
{
    return m_children;
}

inline void BrowserData::addChild(BrowserData *child)
{
    m_children.append(child);
}








#endif // BROWSERDATA_H
