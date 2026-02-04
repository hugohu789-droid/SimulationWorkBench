#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include <vector>
#include <functional>
#include <atomic>

#include "SimulationResult.h"
#include "SimulationConfig.h"


class SimulationEngine
{
public:
    SimulationEngine();

    void setConfig(const SimulationConfig& config);
    const SimulationConfig& config() const;

    // Synchronous calculation
    SimulationResult run();

    SimulationResult runWithCancel(
        const std::atomic<bool>& cancelFlag,
        const std::function<void(int)>& progressCallback = nullptr);

private:
    SimulationConfig m_config;

    void initializeField(std::vector<double>& field);
    void step(const std::vector<double>& current,
              std::vector<double>& next);
};

#endif // SIMULATIONENGINE_H
