#include "ExperimentsBrowser.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QTreeView>

#include "ExperimentsDAO.h"
#include "SpecimensDAO.h"
#include "TubesDAO.h"
#include "UsersDAO.h"
#include "CytometerSettingsDAO.h"
#include "AddNewItemDialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QMessageBox>

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
    m_model = new BrowserDataModel(this);
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

    statusBar = new QStatusBar(mainWidget);
    statusBar->showMessage("Status: IDLE");

    treeView = new QTreeView(mainWidget);
    treeView->setModel(m_model);
    treeView->setSelectionModel(m_theSelection);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_LoginUserIndex = m_model->findIndex(NodeType::User, User::loginUser().name());


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


bool ExperimentsBrowser::isNodeExists(NodeType nodeType, const QString &name, BrowserData *parentNode) {
    if (!parentNode) {
        return false;
    }

    switch (nodeType) {
    case NodeType::Experiment:
        return ExperimentsDAO().isExperimentExists(name, User::loginUser().id());
    case NodeType::Specimen:
        return SpecimensDAO().isSpecimenExists(name, parentNode->nodeId());
    case NodeType::Tube:
        return TubesDAO().isTubeExists(name, parentNode->nodeId());
    case NodeType::Settings:
        return CytometerSettingsDAO().isCytometerSettingsExists(parentNode->nodeType(), parentNode->nodeId());
    default:
        return false;
    }
}


QModelIndex ExperimentsBrowser::getParentForNewNode(NodeType nodeType)
{
    QModelIndex selectedIndex = treeView->selectionModel()->currentIndex();
    if (!selectedIndex.isValid()) {
        return QModelIndex();
    }

    BrowserData *selectedNode = m_model->nodeFromIndex(selectedIndex);
    if (!selectedNode) {
        return QModelIndex();
    }

    switch (nodeType) {
    case NodeType::Experiment:
        if (m_LoginUserIndex.isValid()) {
            return m_LoginUserIndex;
        }
        break;
    case NodeType::Specimen:
        return m_model->getAncestorIndex(selectedIndex, NodeType::Experiment);
        break;
    case NodeType::Tube:
        return m_model->getAncestorIndex(selectedIndex, NodeType::Specimen);
        break;
    case NodeType::Settings:
        if (selectedNode->nodeType() == NodeType::Tube || selectedNode->nodeType() == NodeType::Specimen){

            for (BrowserData *child : selectedNode->children()) {
                if (child->nodeType() == NodeType::Settings) {
                    return QModelIndex();
                }
            }
            return selectedIndex;
        }
        break;
    default:
        return QModelIndex();
        break;
    }

    return QModelIndex();
}

void ExperimentsBrowser::addNewExperiment(bool) {
    addNewNode(NodeType::Experiment);
}

void ExperimentsBrowser::addNewSpecimen(bool) {
    addNewNode(NodeType::Specimen);
}

void ExperimentsBrowser::addNewTube(bool) {
    addNewNode(NodeType::Tube);
}

void ExperimentsBrowser::addNewSettings(bool) {
    addNewNode(NodeType::Settings);
}



void ExperimentsBrowser::addNewNode(NodeType nodeType)
{
    QModelIndex parentIndex = getParentForNewNode(nodeType);
    QString errorMessage;
    if (!parentIndex.isValid()) {
        errorMessage =  QString("%1 %2").arg(NodeTypeHelper::nodeTypeToString(nodeType), tr(" must have a correct parent node"));
        QMessageBox::warning(this, tr("Selection Error"), errorMessage);
        return;
    }

    qDebug() << QString("Add new %1 node under parent: ").arg(NodeTypeHelper::nodeTypeToString(nodeType)) << parentIndex;
    AddNewItemDialog *dlg = new AddNewItemDialog(NodeTypeHelper::nodeTypeToString(nodeType), this);
    if (dlg->exec() == QDialog::Accepted) {
        QString name = dlg->getItemName();
        delete dlg;

        BrowserData *parentNode = m_model->nodeFromIndex(parentIndex);

        if (isNodeExists(nodeType, name, parentNode)) {
            errorMessage =  QString("%1 %2").arg(NodeTypeHelper::nodeTypeToString(nodeType), tr(" already exists. Please choose a different name."));
            QMessageBox::warning(this, tr("Item Exists"), errorMessage);
            return;
        }
        bool insertOk = false;
        int  experimentId = 0;
        switch (nodeType) {
        case NodeType::Experiment:
            ExperimentsDAO().insertExperiment(name, parentNode->nodeId());
            experimentId = ExperimentsDAO().fetchExperimentId(name, parentNode->nodeId());
            if (experimentId > 0) {
                qDebug() << "Insert Experiment OK!" << experimentId;
                insertOk = CytometerSettingsDAO().insertCytometerSettings("CytometerSettings", NodeType::Experiment, experimentId);
            } else {
                insertOk = false;
            }
            if (experimentId > 0 && !insertOk) {
                ExperimentsDAO().deleteExperiment(experimentId);
            }

            break;
        case NodeType::Specimen:
            insertOk = SpecimensDAO().insertSpecimen(name, parentNode->nodeId());
            break;
        case NodeType::Tube:
            insertOk = TubesDAO().insertTube(name, parentNode->nodeId());
            break;
        case NodeType::Settings:
            insertOk = CytometerSettingsDAO().insertCytometerSettings(name, parentNode->nodeType(), parentNode->nodeId());
            break;
        default:
            break;
        }
        if (!insertOk) {
            errorMessage = QString("%1 %2").arg(NodeTypeHelper::nodeTypeToString(nodeType), tr("inserted failed!"));
            QMessageBox::warning(this, tr("Database Error"), errorMessage);
            return;
        }

        if (m_model->insertNewNode(nodeType, name, parentIndex)) {
            treeView->update();
        }
    } else {
        delete dlg;
    }

}

void ExperimentsBrowser::onSelectionChanged(const QItemSelection &selected, const QItemSelection &previous)
{
    if (selected.isEmpty()) {
        return;
    }
    QModelIndex currentIndex = selected.indexes().last();
    currentIndexPath = m_model->getNodePath(currentIndex);
    QStringList pathList;
    for (auto index : currentIndexPath) {
        BrowserData *node = m_model->nodeFromIndex(index);
        pathList << node->nodeName();
    }
    currentPathStr = pathList.join("->");
    statusBar->showMessage(currentPathStr);
}





