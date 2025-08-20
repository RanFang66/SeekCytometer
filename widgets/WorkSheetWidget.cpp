#include "WorkSheetWidget.h"
#include <QToolBar>
#include <QActionGroup>
#include <QVBoxLayout>
#include "WorkSheetsDAO.h"
#include "AddNewPlotDialog.h"
#include "PlotsDAO.h"
// #include "DataManager.h"
// #include "GatesDAO.h"
#include <QInputDialog>
#include "SortingWidget.h"
#include "EventDataManager.h"
#include "GatesModel.h"


WorkSheetWidget::WorkSheetWidget(const QString &title, QWidget *parent)
    : QDockWidget{title, parent},
    m_updateTimer(new QTimer(this)),
    m_active(false),
    m_updateInterval(1000),
    tableView(new QTableView(this)),
    m_model(GatesModel::instance())
{
    initDockWidget();
}

void WorkSheetWidget::setActive(bool active)
{
    setActive(active, m_updateInterval);
}

void WorkSheetWidget::setActive(bool active, int interval)
{
    m_active = active;
    m_updateInterval = interval;
    if (m_active) {
        m_updateTimer->start(m_updateInterval);
    } else {
        m_updateTimer->stop();
    }
}

bool WorkSheetWidget::isActive() const
{
    return m_active;
}


void WorkSheetWidget::addWorkSheetView(int worksheetId)
{
    if (m_activedWorksheetId.contains(worksheetId))
        return;



    // SortingWidget::instance()->updatePopulation(worksheetId);
    m_activedWorksheetId.append(worksheetId);
    WorkSheet workSheet = WorkSheetsDAO().fetchWorkSheet(worksheetId);
    WorkSheetView *workSheetView = new WorkSheetView(workSheet);
    tabWidget->addTab(workSheetView, workSheet.name());
    currentWorkSheetView = workSheetView;
    currentWorkSheetScene = workSheetView->scene();
    connect(currentWorkSheetScene, &WorkSheetScene::finishedDrawingGate, this, &WorkSheetWidget::onFinishedDrawingGate);
    m_model->resetGateModel(worksheetId);

    // for (const Gate& gate : GatesDAO().fetchGates(worksheetId)) {
    //     GatesDAO().deleteGate(gate.id());
    // }

    QList<Plot> plotList = PlotsDAO().fetchPlots(worksheetId);
    // QList<Gate> gateList = GatesDAO().fetchGates(worksheetId);


    for (const Plot &plot : plotList) {
        PlotBase *plotBase = workSheetView->scene()->addNewPlot(plot.plotType(), plot);
        if (plotBase) {
            for (const Gate &gate : m_model->getGateList()) {
                if (gate.xAxisSettingId() == plot.axisXId() && gate.yAxisSettingId() == plot.axisYId()
                    && gate.xMeasurementType() == plot.xMeasurementType() && gate.yMeasurementType() == plot.yMeasurementType()) {
                    workSheetView->scene()->addNewGate(gate.gateType(), gate, plotBase);
                }
            }
        }
    }
}

void WorkSheetWidget::onFinishedDrawingGate(GateItem *gateItem)
{
    if (!currentWorkSheetScene) {
        return;
    }
    bool ok;
    bool addGateOk = false;
    QString gateName = QInputDialog::getText(this, tr("Add New Gate"), tr("Please input gate name and confirm to add this new gate"), QLineEdit::Normal, gateItem->gate().name(), &ok);
    if (ok && !gateName.isEmpty()) {
        gateItem->setGateName(gateName);
        // int gateId = GatesDAO().insertGate(gateItem->gate());
        // if (gateId > 0) {
        //     gateItem->setGateId(gateId);
        //     addGateOk = true;
        // }

        int gateId = m_model->addGate(gateItem->gate());
        if (gateId > 0) {
            gateItem->setGateId(gateId);
            addGateOk = true;
        }
    }

    currentWorkSheetScene->finishDrawingGate(addGateOk);
}

void WorkSheetWidget::initDockWidget()
{
    QWidget *mainWidget = new QWidget(this);

    tableView->setModel(m_model);
    // tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

    QToolBar *toolBar = new QToolBar("ToolBar", mainWidget);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    actionPrint = new QAction("Print", this);
    actionSavePDF = new QAction("Save PDF", this);
    actionSelect = new QAction("Select", this);

    toolBar->addAction(actionPrint);
    toolBar->addAction(actionSavePDF);
    toolBar->addAction(actionSelect);
    toolBar->addSeparator();

    QActionGroup *plotGroup = new QActionGroup(this);
    actionNewHistogram = new QAction("New Histogram", this);
    actionNewScatter = new QAction("New Scatter", this);
    actionNewContour = new QAction("New Contour", this);
    actionNewHistogram->setData(QVariant::fromValue(PlotType::HISTOGRAM_PLOT));
    actionNewScatter->setData(QVariant::fromValue(PlotType::SCATTER_PLOT));
    actionNewContour->setData(QVariant::fromValue(PlotType::CONTOUR_PLOT));
    plotGroup->addAction(actionNewHistogram);
    plotGroup->addAction(actionNewScatter);
    plotGroup->addAction(actionNewContour);
    toolBar->addAction(actionNewHistogram);
    toolBar->addAction(actionNewScatter);
    toolBar->addAction(actionNewContour);
    toolBar->addSeparator();


    QActionGroup *gateGroup = new QActionGroup(this);
    actionNewRectGate = new QAction("New RectGate", this);
    actionNewPolyGate = new QAction("New PolyGate", this);
    actionNewEllipseGate = new QAction("New EllipseGate", this);
    actionNewQuadGate = new QAction("New QuadGate", this);
    actionNewIntervalGate = new QAction("New IntervalGate", this);
    actionNewRectGate->setData(QVariant::fromValue(GateType::RectangleGate));
    actionNewPolyGate->setData(QVariant::fromValue(GateType::PolygonGate));
    actionNewEllipseGate->setData(QVariant::fromValue(GateType::EllipseGate));
    actionNewQuadGate->setData(QVariant::fromValue(GateType::QuadrantGate));
    actionNewIntervalGate->setData(QVariant::fromValue(GateType::IntervalGate));
    gateGroup->addAction(actionNewRectGate);
    gateGroup->addAction(actionNewPolyGate);
    gateGroup->addAction(actionNewEllipseGate);
    gateGroup->addAction(actionNewQuadGate);
    gateGroup->addAction(actionNewIntervalGate);

    toolBar->addAction(actionNewRectGate);
    toolBar->addAction(actionNewPolyGate);
    toolBar->addAction(actionNewEllipseGate);
    toolBar->addAction(actionNewQuadGate);
    toolBar->addAction(actionNewIntervalGate);
    toolBar->addSeparator();


    tabWidget = new QTabWidget(mainWidget);

    tabWidget->setTabsClosable(true);

    QVBoxLayout *layout = new QVBoxLayout(mainWidget);
    layout->addWidget(toolBar);
    layout->addWidget(tabWidget);
    layout->addWidget(tableView);

    mainWidget->setLayout(layout);
    setWidget(mainWidget);
    connect(tabWidget, &QTabWidget::currentChanged, this, &WorkSheetWidget::onCurrentTabChanged);
    connect(plotGroup, &QActionGroup::triggered, this, &WorkSheetWidget::addNewPlot);
    connect(gateGroup, &QActionGroup::triggered, this, &WorkSheetWidget::addNewGate);
    connect(m_updateTimer, &QTimer::timeout, this, &WorkSheetWidget::onUpdateTimerTimeout);

}

void WorkSheetWidget::addPlot(PlotType plotType)
{
    AddNewPlotDialog dialog(plotType, currentWorkSheetView->worksheetId(), this);
    if (dialog.exec() == QDialog::Accepted) {
        Plot plot = dialog.getPlot();
        int plotId = PlotsDAO().insertPlot(plot);
        if (plotId > 0) {
            currentWorkSheetScene->addNewPlot(plotType, plot);
        }
    }
}

void WorkSheetWidget::onCurrentTabChanged(int index)
{
    currentWorkSheetView = qobject_cast<WorkSheetView*>(tabWidget->widget(index));
    if (currentWorkSheetView) {
        currentWorkSheetScene = currentWorkSheetView->scene();
        m_model->resetGateModel(currentWorkSheetView->worksheetId());
    }
}

void WorkSheetWidget::addNewPlot(QAction *action)
{
    if (!currentWorkSheetScene) {
        return;
    }
    PlotType plotType = action->data().value<PlotType>();
    addPlot(plotType);
}


void WorkSheetWidget::addNewGate(QAction *action)
{
    if (!currentWorkSheetScene) {
        return;
    }
    GateType gateType = action->data().value<GateType>();
    currentWorkSheetScene->startDrawingGate(gateType);
}

void WorkSheetWidget::onUpdateTimerTimeout()
{
    // DataManager::instance().processData(currentWorkSheetScene->plots());
    EventDataManager::instance().processData(currentWorkSheetScene->plots());
}


