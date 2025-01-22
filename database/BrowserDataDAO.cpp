#include "BrowserDataDAO.h"


BrowserDataDAO::BrowserDataDAO(QObject *parent)
    : BaseDAO{parent}
{}

TreeNode *BrowserDataDAO::getBrowserData(const User &user)
{
    QList<QVariantMap> treeData;
    QSqlQuery query;
    if (user.isAdmin()) {
        query.prepare(
            R"(
            SELECT
                u.user_id AS user_id,
                u.user_name AS user_name,
                u.created_at AS user_created_at,
                e.experiment_id AS experiment_id,
                e.experiment_name AS experiment_name,
                e.created_at AS experiment_created_at,
                s.specimen_id AS specimen_id,
                s.specimen_name AS specimen_name,
                s.created_at AS specimen_created_at,
                t.tube_id AS tube_id,
                t.tube_name AS tube_name,
                t.created_at AS tube_created_at,
                st.config_id AS config_id,
                st.config_name AS config_name,
                st.parent_type AS config_parent_type,
                st.created_at AS config_created_at
            FROM Users u
            LEFT JOIN Experiments e
                ON u.user_id = e.user_id
            LEFT JOIN Specimens s
                ON e.experiment_id = s.experiment_id
            LEFT JOIN Tubes t
                ON s.specimen_id = t.specimen_id
            LEFT JOIN CytometerSettings st
                ON (
                    (st.parent_type = 'Experiment' AND st.parent_id = e.experiment_id) OR
                    (st.parent_type = 'Specimen' AND st.parent_id = s.specimen_id) OR
                    (st.parent_type = 'Tube' AND st.parent_id = t.tube_id)
                )
            ORDER BY u.user_id, e.experiment_id, s.specimen_id, t.tube_id, st.config_id;
            )"
            );
    } else {
        query.prepare(
            R"(
            SELECT
                u.user_id AS user_id,
                u.user_name AS user_name,
                u.created_at AS user_created_at,
                e.experiment_id AS experiment_id,
                e.experiment_name AS experiment_name,
                e.created_at AS experiment_created_at,
                s.specimen_id AS specimen_id,
                s.specimen_name AS specimen_name,
                s.created_at AS specimen_created_at,
                t.tube_id AS tube_id,
                t.tube_name AS tube_name,
                t.created_at AS tube_created_at,
                st.config_id AS config_id,
                st.config_name AS config_name,
                st.parent_type AS config_parent_type,
                st.created_at AS config_created_at
            FROM Users u
            LEFT JOIN Experiments e
                ON u.user_id = e.user_id
            LEFT JOIN Specimens s
                ON e.experiment_id = s.experiment_id
            LEFT JOIN Tubes t
                ON s.specimen_id = t.specimen_id
            LEFT JOIN CytometerSettings st
                ON (
                    (st.parent_type = 'Experiment' AND st.parent_id = e.experiment_id) OR
                    (st.parent_type = 'Specimen' AND st.parent_id = s.specimen_id) OR
                    (st.parent_type = 'Tube' AND st.parent_id = t.tube_id)
                )
            WHERE u.user_id = :user_id
            ORDER BY u.user_id, e.experiment_id, s.specimen_id, t.tube_id, st.config_id;
            )"
            );
        query.bindValue(":user_id", user.id());
    }

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return nullptr;
    }

    while (query.next()) {
        QVariantMap row;
        row["user_id"] = query.value("user_id");
        row["user_name"] = query.value("user_name");
        row["user_created_at"] = query.value("user_created_at");
        row["experiment_id"] = query.value("experiment_id");
        row["experiment_name"] = query.value("experiment_name");
        row["experiment_created_at"] = query.value("experiment_created_at");
        row["specimen_id"] = query.value("specimen_id");
        row["specimen_name"] = query.value("specimen_name");
        row["specimen_created_at"] = query.value("specimen_created_at");
        row["tube_id"] = query.value("tube_id");
        row["tube_name"] = query.value("tube_name");
        row["tube_created_at"] = query.value("tube_created_at");
        row["config_id"] = query.value("config_id");
        row["config_name"] = query.value("config_name");
        row["config_parent_type"] = query.value("config_parent_type");
        row["config_created_at"] = query.value("config_created_at");

        treeData.append(row);
    }
    return buildTree(treeData);
}

TreeNode *BrowserDataDAO::buildTree(const QList<QVariantMap> &treeData)
{
    TreeNode* rootNode = new TreeNode(NodeType::Root, 0, "Root", QDateTime::currentDateTime());

    QMap<int, TreeNode*> userNodes;
    QMap<int, TreeNode*> experimentNodes;
    QMap<int, TreeNode*> specimenNodes;
    QMap<int, TreeNode*> tubeNodes;
    QMap<int, TreeNode*> configNodes;

    for (const auto& row : treeData) {
        int userId = row["user_id"].toInt();
        if (!userNodes.contains(userId)) {
            TreeNode* userNode = new TreeNode(NodeType::User, row["user_id"].toInt(), row["user_name"].toString(), row["user_created_at"].toDateTime(), rootNode);
            rootNode->addChild(userNode);
            userNodes[userId] = userNode;
        }

        int experimentId = row["experiment_id"].toInt();
        if (!experimentNodes.contains(experimentId)) {
            TreeNode* experimentNode = new TreeNode(NodeType::Experiment, row["experiment_id"].toInt(), row["experiment_name"].toString(), row["experiment_created_at"].toDateTime(), userNodes[userId]);
            userNodes[userId]->addChild(experimentNode);
            experimentNodes[experimentId] = experimentNode;
        }

        int specimenId = row["specimen_id"].toInt();
        if (!specimenNodes.contains(specimenId)) {
            TreeNode* specimenNode = new TreeNode(NodeType::Specimen, row["specimen_id"].toInt(), row["specimen_name"].toString(), row["specimen_created_at"].toDateTime(), experimentNodes[experimentId]);
            experimentNodes[experimentId]->addChild(specimenNode);
            specimenNodes[specimenId] = specimenNode;
        }

        int tubeId = row["tube_id"].toInt();
        if (!tubeNodes.contains(tubeId)) {
            TreeNode* tubeNode = new TreeNode(NodeType::Tube, row["tube_id"].toInt(), row["tube_name"].toString(), row["tube_created_at"].toDateTime(), specimenNodes[specimenId]);
            specimenNodes[specimenId]->addChild(tubeNode);
            tubeNodes[tubeId] = tubeNode;
        }

        int configId = row["config_id"].toInt();
        if (!configNodes.contains(configId)) {
            QString parentType = row["config_parent_type"].toString();
            TreeNode* configNode = nullptr;
            if (parentType == "Experiment") {
                configNode = new TreeNode(NodeType::Config, row["config_id"].toInt(), row["config_name"].toString(), row["config_created_at"].toDateTime(), experimentNodes[experimentId]);
                experimentNodes[experimentId]->addChild(configNode);
            } else if (parentType == "Specimen") {
                configNode = new TreeNode(NodeType::Config, row["config_id"].toInt(), row["config_name"].toString(), row["config_created_at"].toDateTime(), specimenNodes[specimenId]);
                specimenNodes[specimenId]->addChild(configNode);
            } else if (parentType == "Tube") {
                configNode = new TreeNode(NodeType::Config, row["config_id"].toInt(), row["config_name"].toString(), row["config_created_at"].toDateTime(), tubeNodes[tubeId]);
                tubeNodes[tubeId]->addChild(configNode);
            }
            if (configNode)
                configNodes[configId] = configNode;
        }
    }

    return rootNode;
}


