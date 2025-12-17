#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include <QObject>
#include <atomic>

#include "../core/simulationengine.h"

class SimulationConfig;
class SimulationResult;


template <typename T> class QFutureWatcher;

class SimulationController : public QObject
{
    Q_OBJECT
public:
    explicit SimulationController(QObject *parent = nullptr);

public slots:
    void startSimulation(const SimulationConfig& config);
    void cancelSimulation();

signals:
    void simulationStarted();
    void simulationProgress(int percent);
    void simulationFinished(const SimulationResult& result);
    void simulationCancelled();
    void simulationError(const QString& message);

private slots:
    void onFutureFinished();

private:
    SimulationEngine m_engine;

    QFutureWatcher<SimulationResult>* m_watcher = nullptr;
    std::atomic<bool> m_cancelRequested {false};
    bool m_isRunning = false;
};


#endif // SIMULATIONCONTROLLER_H
