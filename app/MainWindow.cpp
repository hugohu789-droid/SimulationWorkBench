#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "ParametersPanel.h"
#include "LogPanel.h"
#include "../viz/SimulationView.h"
#include "SimulationController.h"
// #include "../core/SimulationConfig.h"
// #include "../core/SimulationResult.h"

#include <QDockWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    // 创建核心组件
    m_parametersPanel = new ParametersPanel(this);
    m_logPanel = new LogPanel(this);
    m_simulationView = new SimulationView(this);
    m_controller = new SimulationController(this);

    // 中央区域：仿真可视化视图
    setCentralWidget(m_simulationView);

    // 创建 Dock 面板
    createDockPanels();

    // 创建菜单和工具栏
    createActionsAndMenus();

    // 连接 ParametersPanel → SimulationController
    connect(m_parametersPanel, &ParametersPanel::runRequested,
            this, &MainWindow::onRunRequestedFromPanel);

    // 连接 SimulationController → MainWindow / Log / View
    connect(m_controller, &SimulationController::simulationStarted,
            this, &MainWindow::onSimulationStarted);

    connect(m_controller, &SimulationController::simulationProgress,
            this, &MainWindow::onSimulationProgress);

    connect(m_controller, &SimulationController::simulationFinished,
            this, &MainWindow::onSimulationFinished);

    connect(m_controller, &SimulationController::simulationCancelled,
            this, &MainWindow::onSimulationCancelled);

    connect(m_controller, &SimulationController::simulationError,
            this, &MainWindow::onSimulationError);

    statusBar()->showMessage(tr("Ready"));
    updateUiForSimulationRunning(false);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createDockPanels()
{
    // Parameters Dock
    auto* paramsDock = new QDockWidget(tr("Parameters"), this);
    paramsDock->setObjectName("ParametersDock");
    paramsDock->setWidget(m_parametersPanel);
    paramsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, paramsDock);

    // Log Dock
    auto* logDock = new QDockWidget(tr("Log"), this);
    logDock->setObjectName("LogDock");
    logDock->setWidget(m_logPanel);
    logDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, logDock);
}

void MainWindow::createActionsAndMenus()
{
    // Run / Cancel 动作
    m_runAction = new QAction(tr("Run Simulation"), this);
    m_runAction->setShortcut(QKeySequence(Qt::Key_F5));

    m_cancelAction = new QAction(tr("Cancel"), this);
    m_cancelAction->setShortcut(QKeySequence(Qt::Key_Escape));

    connect(m_runAction, &QAction::triggered,
            this, &MainWindow::onRunActionTriggered);

    connect(m_cancelAction, &QAction::triggered,
            this, &MainWindow::onCancelActionTriggered);

    // 菜单栏
    QMenu* simMenu = menuBar()->addMenu(tr("&Simulation"));
    simMenu->addAction(m_runAction);
    simMenu->addAction(m_cancelAction);

    // 工具栏
    QToolBar* simToolBar = addToolBar(tr("Simulation"));
    simToolBar->addAction(m_runAction);
    simToolBar->addAction(m_cancelAction);
}

void MainWindow::updateUiForSimulationRunning(bool running)
{
    if (m_runAction) {
        m_runAction->setEnabled(!running);
    }
    if (m_cancelAction) {
        m_cancelAction->setEnabled(running);
    }
    if (m_parametersPanel) {
        m_parametersPanel->setEnabled(!running);
    }
}

// ====== Slots ======

void MainWindow::onRunActionTriggered()
{
    if (!m_parametersPanel) {
        return;
    }
    // 从参数面板读取当前配置，启动仿真
    SimulationConfig cfg = m_parametersPanel->currentConfig();
    m_controller->startSimulation(cfg);
}

void MainWindow::onCancelActionTriggered()
{
    if (m_controller) {
        m_controller->cancelSimulation();
    }
}

// 从 ParametersPanel 发出的 runRequested 信号
void MainWindow::onRunRequestedFromPanel(const SimulationConfig& config)
{
    if (m_controller) {
        m_controller->startSimulation(config);
    }
}

void MainWindow::onSimulationStarted()
{
    if (m_logPanel) {
        m_logPanel->appendMessage(tr("Simulation started."));
    }
    statusBar()->showMessage(tr("Simulation running..."));
    updateUiForSimulationRunning(true);
}

void MainWindow::onSimulationProgress(int percent)
{
    statusBar()->showMessage(tr("Simulation running... %1%").arg(percent));
}

void MainWindow::onSimulationFinished(const SimulationResult& result)
{
    if (m_logPanel) {
        m_logPanel->appendMessage(tr("Simulation finished."));
    }

    if (m_simulationView) {
        m_simulationView->setResult(result);
    }

    statusBar()->showMessage(tr("Simulation finished."), 3000);
    updateUiForSimulationRunning(false);
}

void MainWindow::onSimulationCancelled()
{
    if (m_logPanel) {
        m_logPanel->appendMessage(tr("Simulation cancelled."));
    }
    statusBar()->showMessage(tr("Simulation cancelled."), 3000);
    updateUiForSimulationRunning(false);
}

void MainWindow::onSimulationError(const QString& message)
{
    if (m_logPanel) {
        m_logPanel->appendMessage(tr("Simulation error: %1").arg(message));
    }
    statusBar()->showMessage(tr("Simulation error."), 5000);
    updateUiForSimulationRunning(false);
}
