#include "simulationengine.h"

#include <algorithm>
#include <stdexcept>

SimulationEngine::SimulationEngine()
{
}

void SimulationEngine::setConfig(const SimulationConfig& config)
{
    if (config.nx <= 0 || config.ny <= 0) {
        throw std::invalid_argument("Grid dimensions must be positive.");
    }
    if (config.timeSteps < 0) {
        throw std::invalid_argument("Time steps must be non-negative.");
    }
    if (config.diffusionCoeff < 0.0) {
        throw std::invalid_argument("Diffusion coefficient must be non-negative.");
    }

    m_config = config;
}

const SimulationConfig& SimulationEngine::config() const
{
    return m_config;
}

SimulationResult SimulationEngine::run()
{
    std::atomic<bool> dummyCancel(false);
    return runWithCancel(dummyCancel, nullptr);
}

void SimulationEngine::initializeField(std::vector<double>& field)
{
    const int nx = m_config.nx;
    const int ny = m_config.ny;

    std::fill(field.begin(), field.end(), 0.0);

    // Central hotspot (initial conditions)
    const int cx = nx / 2;
    const int cy = ny / 2;

    field[static_cast<size_t>(cy * nx + cx)] = 1.0;
}

void SimulationEngine::step(const std::vector<double>& current,
                            std::vector<double>& next)
{
    const int nx = m_config.nx;
    const int ny = m_config.ny;
    const double alpha = m_config.diffusionCoeff;

    for (int j = 1; j < ny - 1; ++j) {
        for (int i = 1; i < nx - 1; ++i) {

            const size_t idx = static_cast<size_t>(j * nx + i);

            const double center = current[idx];
            const double left   = current[idx - 1];
            const double right  = current[idx + 1];
            const double down   = current[idx - nx];
            const double up     = current[idx + nx];

            next[idx] = center + alpha *
                                     (left + right + up + down - 4.0 * center);
        }
    }
}

SimulationResult SimulationEngine::runWithCancel(
    const std::atomic<bool>& cancelFlag,
   const std::function<void(int)>& progressCallback)
{
    const int nx = m_config.nx;
    const int ny = m_config.ny;
    const int steps = m_config.timeSteps;
    const double alpha = m_config.diffusionCoeff;

    SimulationResult result(nx, ny);

    std::vector<double> current(static_cast<size_t>(nx * ny), 0.0);
    std::vector<double> next(static_cast<size_t>(nx * ny), 0.0);

    initializeField(current);

    for (int step = 0; step < steps; ++step) {

        // Key: Check the cancellation flag once at each time step.
        if (cancelFlag.load()) {
            // End the loop early, keeping the current result as an "interim result" or an empty result.
            break;
        }

        for (int j = 1; j < ny - 1; ++j) {
            for (int i = 1; i < nx - 1; ++i) {

                const size_t idx = static_cast<size_t>(j * nx + i);

                const double center = current[idx];
                const double left   = current[idx - 1];
                const double right  = current[idx + 1];
                const double down   = current[idx - nx];
                const double up     = current[idx + nx];

                next[idx] = center + alpha *
                                         (left + right + up + down - 4.0 * center);
            }
        }

        std::swap(current, next);

        // Progress callback (e.g., report every 10 steps)
        if (progressCallback && steps > 0) {
            if (step % 10 == 0 || step == steps - 1) {
                int percent = static_cast<int>(100.0 * (step + 1) / steps);
                progressCallback(percent);
            }
        }
    }

    // Copy the current result
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            const size_t idx = static_cast<size_t>(j * nx + i);
            result.setValue(i, j, current[idx]);
        }
    }

    return result;
}
