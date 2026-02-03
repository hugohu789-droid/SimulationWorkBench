cmake_minimum_required(VERSION 3.19)
project(SimulationWorkBench LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Widgets
    OpenGL
    OpenGLWidgets
    Concurrent
    LinguistTools
)

find_package(VTK REQUIRED COMPONENTS
    CommonCore
    CommonDataModel
    RenderingCore
    RenderingOpenGL2
    InteractionStyle
    GUISupportQt
    RenderingAnnotation
)

enable_testing()
add_subdirectory(core)
add_subdirectory(viz)
add_subdirectory(app)
add_subdirectory(tests)

qt_standard_project_setup()

qt_add_translations(
    TARGETS SimulationWorkBench
    TS_FILES SimulationWorkBench_en_NZ.ts
)
