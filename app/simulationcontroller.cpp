#include "SimulationController.h"


#include <QtConcurrent>          // QtConcurrent::run
#include <QFuture>
#include <QFutureWatcher>
#include <QString>

SimulationController::SimulationController(QObject* parent)
    : QObject(parent)
    , m_watcher(new QFutureWatcher<SimulationResult>(this))
{
    // The slot function is triggered when the background task completes.
    connect(m_watcher, &QFutureWatcher<SimulationResult>::finished,
            this, &SimulationController::onFutureFinished);
}

void SimulationController::startSimulation(const SimulationConfig& config)
{
    // If it's already running, discard new requests.
    if (m_isRunning) {
        emit simulationError(QStringLiteral("Simulation is already running."));
        return;
    }

    try {
        m_engine.setConfig(config);
    } catch (const std::exception& ex) {
        emit simulationError(QString::fromUtf8(ex.what()));
        return;
    }

    m_cancelRequested.store(false);
    m_isRunning = true;

    emit simulationStarted();
    emit simulationProgress(0);

    // Use QtConcurrent::run to run SimulationEngine::run() in a background thread.
    // We only run one simulation at a time.
    QFuture<SimulationResult> future = QtConcurrent::run(
        [this]() -> SimulationResult {
            return m_engine.runWithCancel(
                m_cancelRequested,
                // Progress callback: It is safe to emit signals from a background thread.
                [this](int percent) {
                    emit simulationProgress(percent);
                }
                );
        }
        );

    m_watcher->setFuture(future);
}

void SimulationController::cancelSimulation()
{
    if (!m_isRunning) {
        return;
    }

    // Mark it as cancelled, don't rush to issue a cancelled message.
    m_cancelRequested.store(true);
}

void SimulationController::onFutureFinished()
{
    m_isRunning = false;

    const QFuture<SimulationResult>& future = m_watcher->future();

    if (m_cancelRequested.load()) {
        // The user has sent a cancellation request → it is considered that the cancellation is complete.
        emit simulationCancelled();
        emit simulationProgress(100);
        return;
    }

    if (future.isCanceled()) {
        emit simulationCancelled();
        emit simulationProgress(100);
        return;
    }

    try {
        SimulationResult result = future.result();
        emit simulationFinished(result);
        emit simulationProgress(100);
    } catch (const std::exception& ex) {
        emit simulationError(QString::fromUtf8(ex.what()));
    }
}


