cmake_minimum_required(VERSION 3.19)
project(SimulationWorkBench LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === 新增：定义一个开关，默认开启 GUI ===
option(BUILD_GUI "Build GUI application and visualization modules" ON)

enable_testing()

add_subdirectory(core)

# === 单元测试 (Tests) 永远编译 ===
# 前提是你已经按照建议创建了 tests 目录
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()

# === GUI 相关模块 (仅在 BUILD_GUI 为 ON 时编译) ===
if(BUILD_GUI)
    message(STATUS "Building with GUI support (Qt & VTK)...")
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

    add_subdirectory(viz)
    add_subdirectory(app)

    qt_standard_project_setup()

    qt_add_translations(
        TARGETS SimulationWorkBench
        TS_FILES SimulationWorkBench_en_NZ.ts
    )

else()
    message(STATUS "Building CORE ONLY (Headless mode). Skipping Qt & VTK.")
endif()
