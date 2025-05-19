#ifndef BROWSERDATADAO_H
#define BROWSERDATADAO_H

#include "BaseDAO.h"

#include "BrowserData.h"
#include "User.h"


// SQL for create table BrowserData
// CREATE TYPE NodeType AS ENUM ('Root', 'User', 'Experiment', 'Specimen', 'Tube', 'Settings', 'Worksheet');
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



class BrowserDataDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit BrowserDataDAO(QObject *parent = nullptr);

    int  insertNode(NodeType nodeType, const QString &nodeName, int parentId);
    bool updateNode(const BrowserData &node);
    bool deleteNode(int nodeId);
    bool deleteNode(const BrowserData &node);
    bool deleteTree(BrowserData *node);
    bool deleteTree(int nodeId);
    bool deleteTree(const BrowserData &node);


    BrowserData *fetchBrowserData(const User &user);
    BrowserData *fetechNode(BrowserData *parentNode, NodeType nodeType, int nodeId);

private:
    BrowserData *buildTree(const QList<QVariantMap> &treeData);
    static const QString queryUserData;
};

#endif // BROWSERDATADAO_H
