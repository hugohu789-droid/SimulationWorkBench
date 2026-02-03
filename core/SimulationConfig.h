#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

class SimulationConfig
{
public:
    SimulationConfig();

public:
    int nx = 50;
    int ny = 50;
    int timeSteps = 1000;
    double diffusionCoeff = 0.1;
};

#endif // SIMULATIONCONFIG_H
