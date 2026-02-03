#ifndef SIMULATIONRESULT_H
#define SIMULATIONRESULT_H

#include <vector>

class SimulationResult
{
public:
    SimulationResult() = default;
    SimulationResult(int nx, int ny);

    int nx() const { return m_nx; }
    int ny() const { return m_ny; }

    double value(int i, int j) const;
    void setValue(int i, int j, double v);

    const std::vector<double>& data() const { return m_data; }

private:
    int m_nx = 0;
    int m_ny = 0;
    std::vector<double> m_data; // size = nx * ny
};

#endif // SIMULATIONRESULT_H
