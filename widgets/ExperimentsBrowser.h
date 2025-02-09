#ifndef EXPERIMENTSBROWSER_H
#define EXPERIMENTSBROWSER_H

#include <QDockWidget>
#include <QObject>
#include <QItemSelectionModel>
#include <QStatusBar>
#include <QTreeView>
#include "BrowserDataModel.h"

class ExperimentsBrowser : public QDockWidget
{
    Q_OBJECT
public:
    explicit ExperimentsBrowser(const QString &title, QWidget *parent = nullptr);
    ~ExperimentsBrowser();

    struct CurrentIndexPath {
        QModelIndex currentUser;
        QModelIndex currentExperiment;
        QModelIndex currentSpecimen;
        QModelIndex currentTube;
        QModelIndex currentSettings;
    };

signals:

private:
    BrowserDataModel *m_model;
    QItemSelectionModel *m_theSelection;
    QAction *addExperiment;
    QAction *addSpecimen;
    QAction *addTube;
    QAction *addSettings;

    QTreeView *treeView;

    QStatusBar *statusBar;
    BrowserData *currentNode;
    QList<BrowserData*> currentNodePath;

    QModelIndex m_LoginUserIndex;
    QList<QModelIndex> currentIndexPath;
    CurrentIndexPath currentIndexList;
    QString currentPathStr;


    void initDockWidget();

    bool isNodeExists(NodeType nodeType, const QString &name, BrowserData *parentNode);

    QModelIndex getParentForNewNode(NodeType nodeType);
    void addNewNode(NodeType nodeType);

private slots:
    void onSelectionChanged(const QItemSelection &current, const QItemSelection &previous);
    void addNewExperiment(bool checked = false);
    void addNewSpecimen(bool checked = false);
    void addNewTube(bool checked = false);
    void addNewSettings(bool checked = false);
};

#endif // EXPERIMENTSBROWSER_H
