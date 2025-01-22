#include "ExperimentsBrowser.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QTreeView>

#include "TreeModel.h"

ExperimentsBrowser::ExperimentsBrowser(const QString &title, QWidget *parent)
    : QDockWidget{title, parent}
{
    initDockWidget();
}

ExperimentsBrowser::~ExperimentsBrowser()
{

}

void ExperimentsBrowser::initDockWidget()
{
    m_model = new TreeModel(this);
    m_theSelection = new QItemSelectionModel(m_model);

    QWidget *mainWidget = new QWidget();

    QToolBar *toolBar = new QToolBar("ToolBar", mainWidget);
    addExperiment = new QAction("New Experiment", mainWidget);
    addSpecimen = new QAction("New Specimen", mainWidget);
    addTube = new QAction("New Tube", mainWidget);
    addSettings = new QAction("New CytometerSettings", mainWidget);

    toolBar->addAction(addExperiment);
    toolBar->addAction(addSpecimen);
    toolBar->addAction(addTube);
    toolBar->addAction(addSettings);

    QStatusBar *statusBar = new QStatusBar(mainWidget);
    statusBar->showMessage("Status: IDLE");

    QTreeView *treeView = new QTreeView(mainWidget);  
    treeView->setModel(m_model);
    treeView->setSelectionModel(m_theSelection);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QVBoxLayout *layout = new QVBoxLayout(mainWidget);
    layout->addWidget(toolBar);
    layout->addWidget(treeView);
    layout->addWidget(statusBar);

    treeView->setWindowTitle("Experiment Data");
    treeView->resize(800, 1200);
    treeView->show();

    mainWidget->setLayout(layout);
    setWidget(mainWidget);

    connect(m_theSelection, &QItemSelectionModel::selectionChanged, this, &ExperimentsBrowser::onSelectionChanged);

    connect(addExperiment, &QAction::triggered, this, &ExperimentsBrowser::addNewExperiment);
    connect(addSpecimen, &QAction::triggered, this, &ExperimentsBrowser::addNewSpecimen);
    connect(addTube, &QAction::triggered, this, &ExperimentsBrowser::addNewTube);
    connect(addSettings, &QAction::triggered, this, &ExperimentsBrowser::addNewSettings);

}

void ExperimentsBrowser::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (current.isValid()) {
        TreeNode *node = m_model->nodeFromIndex(current);
        switch(node->getType()) {
        case NodeType::User:
            currentUser = node;
            currentExperiment = nullptr;
            currentSpecimen = nullptr;
            currentTube = nullptr;
            currentSettings = nullptr;
            break;
        case NodeType::Experiment:
            currentExperiment = node;
            currentSettings = currentExperiment->getSettings();
            currentSpecimen = nullptr;
            currentTube = nullptr;
            break;
        case NodeType::Specimen:
            currentSpecimen = node;
            currentExperiment = currentSpecimen->getParent();
            currentTube = nullptr;
            currentSettings = currentSpecimen->getSettings();
            break;
        case NodeType::Tube:
            currentTube = node;
            currentSpecimen = currentTube->getParent();
            currentExperiment = currentSpecimen->getParent();
            currentSettings = currentTube->getSettings();
            break;
        case NodeType::Config:
            currentSettings = node;
            switch(currentSettings->getParent()->getType()) {
            case NodeType::Experiment:
                currentExperiment = currentSettings->getParent();
                currentSpecimen = nullptr;
                currentTube = nullptr;
                break;
            case NodeType::Specimen:
                currentSpecimen = currentSettings->getParent();
                currentExperiment = currentSpecimen->getParent();
                currentTube = nullptr;
                break;
            case NodeType::Tube:
                currentTube = currentSettings->getParent();
                currentSpecimen = currentTube->getParent();
                currentExperiment = currentSpecimen->getParent();
                break;
            default:
                break;
            }

            break;
        default:
            break;
        }
    }
}

void ExperimentsBrowser::addNewExperiment(bool checked)
{
    m_model->addNewNode(NodeType::Experiment, currentExperiment->getParent());
}

void ExperimentsBrowser::addNewSpecimen(bool checked)
{
    if (currentExperiment) {
        m_model->addNewNode(NodeType::Specimen, currentExperiment);
    }
}

void ExperimentsBrowser::addNewTube(bool checked)
{
    if (currentSpecimen) {
        m_model->addNewNode(NodeType::Tube, currentSpecimen);
    }
}

void ExperimentsBrowser::addNewSettings(bool checked)
{

}





