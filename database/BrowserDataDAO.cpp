#include "BrowserDataDAO.h"

const QString BrowserDataDAO::queryUserData = R"(
        WITH RECURSIVE UserHierarchy AS (
            SELECT
                id,
                parent_id,
                node_name,
                node_type,
                node_id,
                depth,
                created_at,
                updated_at
            FROM BrowserData
            WHERE node_type = 'User' AND node_id = :userId

            UNION ALL

            SELECT
                bd.id,
                bd.parent_id,
                bd.node_name,
                bd.node_type,
                bd.node_id,
                bd.depth,
                bd.created_at,
                bd.updated_at
            FROM BrowserData bd
            INNER JOIN UserHierarchy uh ON bd.parent_id = uh.id
        )
        SELECT * FROM UserHierarchy ORDER BY depth, node_type, id;
    )";

BrowserDataDAO::BrowserDataDAO(QObject *parent)
    : BaseDAO{parent}
{}

BrowserData *BrowserDataDAO::fetchBrowserData(const User &user)
{
    QSqlQuery query;
    if (user.isAdmin()) {
        query.prepare("SELECT * FROM BrowserData");
    } else {
        query.prepare(queryUserData);
        query.bindValue(":userId", user.id());
    }

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return nullptr;
    }
    BrowserData *rootNode = new BrowserData(0, -1, "Root", NodeType::Root, 0, 0);

    QHash<int, BrowserData*> nodeMap;
    nodeMap[0] = rootNode;
    while(query.next()) {
        int id = query.value("id").toInt();
        int parentId = query.value("parent_id").toInt();
        QString nodeName = query.value("node_name").toString();
        QString nodeTypeStr = query.value("node_type").toString();
        NodeType nodeType = NodeTypeHelper::stringToNodeType(nodeTypeStr);
        int nodeId = query.value("node_id").toInt();
        int depth = query.value("depth").toInt();
        QDateTime createdAt = query.value("created_at").toDateTime();
        QDateTime updatedAt = query.value("updated_at").toDateTime();
        BrowserData *parentNode = nodeMap.value(parentId, nullptr);
        if (parentNode) {
            BrowserData *node = new BrowserData(id, parentId, nodeName, nodeType, nodeId, depth, createdAt, updatedAt, parentNode);
            parentNode->addChild(node);
            nodeMap[id] = node;
        }
    }
    return rootNode;
}

BrowserData *BrowserDataDAO::fetechNode(BrowserData *parentNode,  NodeType nodeType, int nodeId)
{
    if (!parentNode) {
        return nullptr;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM BrowserData WHERE parent_id = :parentId AND node_type = :nodeType AND node_id = :nodeId");
    query.bindValue(":parentId", parentNode->id());
    query.bindValue(":nodeType", NodeTypeHelper::nodeTypeToString(nodeType));
    query.bindValue(":nodeId", nodeId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return nullptr;
    }

    if (query.next()) {
        int id = query.value("id").toInt();
        int parentId = query.value("parent_id").toInt();
        QString nodeName = query.value("node_name").toString();
        QString nodeTypeStr = query.value("node_type").toString();
        NodeType nodeType = NodeTypeHelper::stringToNodeType(nodeTypeStr);
        int nodeId = query.value("node_id").toInt();
        int depth = query.value("depth").toInt();
        QDateTime createdAt = query.value("created_at").toDateTime();
        QDateTime updatedAt = query.value("updated_at").toDateTime();
        return new BrowserData(id, parentId, nodeName, nodeType, nodeId, depth, createdAt, updatedAt, parentNode);
    }
    return nullptr;
}





