#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ParametersPanel;
class LogPanel;
class SimulationView;
class SimulationController;
class SimulationConfig;
class SimulationResult;
class QString;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 菜单/工具栏的 Run 按钮触发
    void onRunActionTriggered();
    void onCancelActionTriggered();

    // SimulationController 的信号回调
    void onRunRequestedFromPanel(const SimulationConfig& config);
    void onSimulationStarted();
    void onSimulationProgress(int percent);
    void onSimulationFinished(const SimulationResult& result);
    void onSimulationCancelled();
    void onSimulationError(const QString& message);

private:
    void createDockPanels();
    void createActionsAndMenus();
    void updateUiForSimulationRunning(bool running);

private:
    Ui::MainWindow* ui = nullptr;

    ParametersPanel* m_parametersPanel = nullptr;
    LogPanel* m_logPanel = nullptr;
    SimulationView* m_simulationView = nullptr;
    SimulationController* m_controller = nullptr;

    QAction* m_runAction = nullptr;
    QAction* m_cancelAction = nullptr;
};
#endif // MAINWINDOW_H
