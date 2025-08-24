#ifndef TUBESELECTMODEL_H
#define TUBESELECTMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include "BrowserDataModel.h"
#include <QTreeView>
class TubeSelectModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TubeSelectModel(BrowserDataModel *model, QObject *parent = nullptr);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void resetTubeSelectModel(QTreeView *treeView);


private:
    QList<QModelIndex> m_visibleRows;
    BrowserDataModel *m_browserDataModel;


    void collectVisibleIndexes(const QModelIndex &parent, QList<QModelIndex> &out, QTreeView *treeView);
};

#endif // TUBESELECTMODEL_H
