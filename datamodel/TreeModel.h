#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <TreeNode.h>
#include "BrowserDataDAO.h"


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    TreeNode *nodeFromIndex(const QModelIndex &index) const;

public slots:
    void addNewNode(NodeType type, TreeNode *parent);

private:
    TreeNode *m_rootNode;

    enum Column {
        COLUMN_NAME,
        COLUMN_TYPE,
        COLUMN_DATE,
        // COLUMN_SIZE,
    };
    const QStringList m_headers = {tr("Name"), tr("Type"), tr("Date"), tr("Size")};
    BrowserDataDAO m_dao;
};

#endif // TREEMODEL_H
