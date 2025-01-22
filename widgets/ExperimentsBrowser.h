#ifndef EXPERIMENTSBROWSER_H
#define EXPERIMENTSBROWSER_H

#include <QDockWidget>
#include <QObject>
#include "TreeModel.h"
#include <QItemSelectionModel>

class ExperimentsBrowser : public QDockWidget
{
    Q_OBJECT
public:
    explicit ExperimentsBrowser(const QString &title, QWidget *parent = nullptr);
    ~ExperimentsBrowser();

signals:

private:
    TreeModel *m_model;
    QItemSelectionModel *m_theSelection;
    QAction *addExperiment;
    QAction *addSpecimen;
    QAction *addTube;
    QAction *addSettings;

    TreeNode *currentUser;
    TreeNode *currentExperiment;
    TreeNode *currentSpecimen;
    TreeNode *currentTube;
    TreeNode *currentSettings;

    void initDockWidget();

private slots:
    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void addNewExperiment(bool checked = false);
    void addNewSpecimen(bool checked = false);
    void addNewTube(bool checked = false);
    void addNewSettings(bool checked = false);
};

#endif // EXPERIMENTSBROWSER_H
