#ifndef BROWSERVIEW_H
#define BROWSERVIEW_H

#include <QWidget>
#include <QTreeView>
#include <QTableWidget>
#include "BrowserDataModel.h"

#include "TubeSelectModel.h"

class BrowserView : public QWidget
{
    Q_OBJECT
public:
    explicit BrowserView(QWidget *parent = nullptr);

    QTreeView *treeView() const {
        return m_treeView;
    }
    BrowserDataModel *browserModel() const {
        return m_browserModel;
    }
    QItemSelectionModel *selectionModel() const {
        return m_theSelection;
    }

signals:


private slots:
    void syncRows();

private:
    QTreeView *m_treeView;
    QTableView *m_tableButton;
    BrowserDataModel *m_browserModel;
    QItemSelectionModel *m_theSelection;
    TubeSelectModel *m_selectModel;


    void initBrowserView();

    void collectVisibleIndexes(const QModelIndex &parent, QList<QModelIndex> &out);
};

#endif // BROWSERVIEW_H
