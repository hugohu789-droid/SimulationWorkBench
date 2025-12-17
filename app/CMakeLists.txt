set(SOURCES
    main.cpp
    MainWindow.cpp
    ParametersPanel.cpp
    LogPanel.cpp
    SimulationController.cpp
)

set(HEADERS
    MainWindow.h
    ParametersPanel.h
    LogPanel.h
    SimulationController.h
)

set(UI_FILES
    MainWindow.ui
    ParametersPanel.ui
    LogPanel.ui
)

qt_add_executable(SimulationWorkBench
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

target_link_libraries(SimulationWorkBench
    PRIVATE
        Qt6::Widgets
        Qt6::Concurrent
        core
        viz
)
