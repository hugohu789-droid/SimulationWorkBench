#include "simulationresult.h"

#include <stdexcept>

SimulationResult::SimulationResult(int nx, int ny)
    : m_nx(nx)
    , m_ny(ny)
    , m_data(static_cast<size_t>(nx * ny), 0.0)
{
    if (nx <= 0 || ny <= 0) {
        throw std::invalid_argument("SimulationResult dimensions must be positive.");
    }
}

double SimulationResult::value(int i, int j) const
{
    if (i < 0 || i >= m_nx || j < 0 || j >= m_ny) {
        throw std::out_of_range("SimulationResult::value index out of range.");
    }

    return m_data[static_cast<size_t>(j * m_nx + i)];
}

void SimulationResult::setValue(int i, int j, double v)
{
    if (i < 0 || i >= m_nx || j < 0 || j >= m_ny) {
        throw std::out_of_range("SimulationResult::setValue index out of range.");
    }

    m_data[static_cast<size_t>(j * m_nx + i)] = v;
}
