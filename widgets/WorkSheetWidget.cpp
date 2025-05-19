#include "WorkSheetWidget.h"
#include <QToolBar>
#include <QACtionGroup>
#include <QVBoxLayout>
#include "WorkSheetsDAO.h"
#include "AddNewPlotDialog.h"
#include "PlotsDAO.h"
#include "DataManager.h"
#include "GatesDAO.h"
#include <QInputDialog>

WorkSheetWidget::WorkSheetWidget(const QString &title, QWidget *parent)
    : QDockWidget{title, parent}, m_updateTimer(new QTimer(this)), m_active(false), m_updateInterval(1000)
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
    WorkSheet workSheet = WorkSheetsDAO().fetchWorkSheet(worksheetId);
    WorkSheetView *workSheetView = new WorkSheetView(workSheet);
    tabWidget->addTab(workSheetView, workSheet.name());
    currentWorkSheetView = workSheetView;
    currentWorkSheetScene = workSheetView->scene();
    connect(currentWorkSheetScene, &WorkSheetScene::finishedDrawingGate, this, &WorkSheetWidget::onFinishedDrawingGate);
    QList<Plot> plotList = PlotsDAO().fetchPlots(worksheetId);
    for (const Plot &plot : plotList) {
        workSheetView->scene()->addNewPlot(plot.plotType(), plot);
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
        int gateId = GatesDAO().insertGate(gateItem->gate());
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
    DataManager::instance().processData(currentWorkSheetScene->plots());
}


