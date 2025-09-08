#include "BrowserView.h"
#include <QHBoxLayout>
#include "TubeButtonDelegate.h"
#include <QStandardItemModel>
#include <QHeaderView>
#include <QScrollBar>

BrowserView::BrowserView(QWidget *parent)
    : QWidget{parent}
{
    initBrowserView();
}

void BrowserView::syncRows()
{
    m_selectModel->resetTubeSelectModel(m_treeView);
}

void BrowserView::initBrowserView()
{
    auto *layout = new QHBoxLayout(this);

    m_tableButton = new QTableView(this);
    m_treeView = new QTreeView(this);

    m_browserModel = new BrowserDataModel(this);
    m_theSelection = new QItemSelectionModel(m_browserModel);

    m_selectModel = new TubeSelectModel(m_browserModel, this);


    m_treeView->setModel(m_browserModel);
    m_treeView->setSelectionModel(m_theSelection);
    m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_treeView->setWindowTitle("Experiment Data");
    m_treeView->show();
    m_treeView->setHeaderHidden(false);
    m_treeView->setStyleSheet(R"(
    QTreeView::item {
        height: 25px;
        padding: 0px;
    }
    QTreeView::item:selected {
        background-color: #e6f3ff;
        color: black;
    }
    )");



    m_tableButton->setModel(m_selectModel);
    m_tableButton->verticalHeader()->setVisible(false);
    m_tableButton->horizontalHeader()->setVisible(true);
    m_tableButton->setFixedWidth(25);
    m_tableButton->verticalHeader()->setDefaultSectionSize(25);
    m_tableButton->setItemDelegate(new TubeButtonDelegate(m_tableButton));

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_tableButton);
    layout->addWidget(m_treeView);

    connect(m_treeView->verticalScrollBar(), &QScrollBar::valueChanged, m_tableButton->verticalScrollBar(), &QScrollBar::setValue);
    connect(m_tableButton->verticalScrollBar(), &QScrollBar::valueChanged, m_treeView->verticalScrollBar(), &QScrollBar::setValue);

    connect(m_browserModel, &QStandardItemModel::rowsInserted, this, &BrowserView::syncRows);
    connect(m_browserModel, &QStandardItemModel::rowsRemoved,  this, &BrowserView::syncRows);
    connect(m_treeView, &QTreeView::expanded, this, &BrowserView::syncRows);
    connect(m_treeView, &QTreeView::collapsed, this, &BrowserView::syncRows);

    syncRows();
}

void BrowserView::collectVisibleIndexes(const QModelIndex &parent, QList<QModelIndex> &out)
{
    int rows = m_browserModel->rowCount(parent);
    for (int r = 0; r < rows; r++) {
        QModelIndex idx = m_browserModel->index(r, 0, parent);
        out << idx;
        if (m_treeView->isExpanded(idx)) {
            collectVisibleIndexes(idx, out);
        }
    }
}
