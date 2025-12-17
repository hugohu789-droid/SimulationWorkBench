add_library(core
    SimulationConfig.cpp
    SimulationEngine.cpp
    SimulationResult.cpp
    SimulationConfig.h
    SimulationEngine.h
    SimulationResult.h
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
