#include "BrowserDataDAO.h"
// CREATE TYPE NodeType AS ENUM ('User', 'Experiment', 'Specimen', 'Tube', 'Setting');
// CREATE TABLE BrowserData (
//     id SERIAL PRIMARY KEY NOT NULL,
//     parent_id INT,
//     node_name VARCHAR(64) NOT NULL,
//     node_type NodeType NOT NULL,
//     node_id INT NOT NULL,
//     depth INT NOT NULL,
//     created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
//     updated_at  TIMESTAMP DEFAULT CURRENT_TIMESTAMP
//     );


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

BrowserData *BrowserDataDAO::getBrowserData(const User &user)
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

BrowserData *BrowserDataDAO::getNewNode(NodeType nodeType, const QString &nodeName, BrowserData *parentNode)
{
    if (!parentNode) {
        return nullptr;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM BrowserData WHERE node_type=:node_type AND parent_id=:parent_id AND node_name=:node_name");
    query.bindValue(":node_type", NodeTypeHelper::nodeTypeToString(nodeType));
    query.bindValue(":parent_id", parentNode->id());
    query.bindValue(":node_name", nodeName);
    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return nullptr;
    }
    if (query.next()) {
        int id = query.value("id").toInt();
        int parentId = query.value("parent_id").toInt();
        int nodeId = query.value("node_id").toInt();
        int depth = query.value("depth").toInt();
        QDateTime createdAt = query.value("created_at").toDateTime();
        QDateTime updatedAt = query.value("updated_at").toDateTime();
        BrowserData *node = new BrowserData(id, parentId, nodeName, nodeType, nodeId, depth, createdAt, updatedAt, parentNode);
        return node;
    } else {
        return nullptr;
    }
}





