#ifndef BROWSERDATAMODEL_H
#define BROWSERDATAMODEL_H

#include <QAbstractItemModel>
#include "BrowserData.h"

class BrowserDataModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit BrowserDataModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    BrowserData *nodeFromIndex(const QModelIndex &index) const;

    QModelIndex indexFromNode(BrowserData *node) const;

    QModelIndex findIndex(NodeType nodeType, const QString &nodeName) const;

    QModelIndex findIndexRecursive(const QModelIndex &parent, NodeType nodeType, const QString &nodeName) const;

    QModelIndex getAncestorIndex(const QModelIndex &current, NodeType ancestorNodeType) const;

    bool updateDataFromDatabase();

    bool insertNewNode(NodeType nodeType, const QString &nodeName, const QModelIndex &parent);
    bool insertCytometerSettings(BrowserData *experimentNode);

    QList<QModelIndex> getNodePath(const QModelIndex &Index);

private:
    BrowserData *m_rootNode;

    enum Column {
        COLUMN_NAME,
        COLUMN_TYPE,
        COLUMN_DATE,
    };
    const QStringList m_headers = {tr("Name"), tr("Type"), tr("Date")};

    void deleteTree(BrowserData *node);

};

#endif // BROWSERDATAMODEL_H
