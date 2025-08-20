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
    static ExperimentsBrowser *instance()
    {
        static ExperimentsBrowser instance("Experiments Browser");
        return &instance;
    }


    ~ExperimentsBrowser();

    // struct CurrentIndexPath {
    //     QModelIndex currentUser;
    //     QModelIndex currentExperiment;
    //     QModelIndex currentSpecimen;
    //     QModelIndex currentTube;
    //     QModelIndex currentSettings;
    // };

signals:
    void experimentSelected(int experimentId);
    void worksheetSelected(int worksheetId);
    void settingsSelected(int settingsId);

private:
    explicit ExperimentsBrowser(const QString &title, QWidget *parent = nullptr);
    ExperimentsBrowser &operator=(const ExperimentsBrowser &) = delete;
    ExperimentsBrowser(const ExperimentsBrowser &) = delete;

    BrowserDataModel *m_model;
    QItemSelectionModel *m_theSelection;
    QAction *addExperiment;
    QAction *addSpecimen;
    QAction *addTube;
    QAction *addSettings;
    QAction *addWorkSheet;
    QAction *expandAll;
    QAction *collapseAll;

    QTreeView *treeView;

    QStatusBar *statusBar;
    BrowserData *currentNode;
    QList<BrowserData*> currentNodePath;

    QModelIndex m_LoginUserIndex;
    QList<QModelIndex> currentIndexPath;
    // CurrentIndexPath currentIndexList;
    QString currentPathStr;


    void initDockWidget();

    bool isNodeExists(NodeType nodeType, const QString &name, BrowserData *parentNode);

    QModelIndex getParentForNewNode(NodeType nodeType);

    QModelIndex insertNewNode(NodeType nodeType, const QString &name, const QModelIndex &parent);

    int insertNewNodeToDB(NodeType nodeType, BrowserData *parentNode, const QString &nodeName);

private slots:
    void onAddNewNodeTriggered(QAction *action);
    void onSelectionChanged(const QItemSelection &current, const QItemSelection &previous);
};

#endif // EXPERIMENTSBROWSER_H
