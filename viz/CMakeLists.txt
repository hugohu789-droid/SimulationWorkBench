add_library(viz
    SimulationView.cpp
    VTKAdapter.cpp
    SimulationView.h
    VTKAdapter.h
)

target_include_directories(viz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(viz
    PUBLIC
        core
        Qt6::Widgets
        Qt6::OpenGLWidgets
        VTK::CommonCore
        VTK::CommonDataModel
        VTK::FiltersCore
        VTK::RenderingCore
        VTK::InteractionStyle
        VTK::RenderingOpenGL2
        VTK::GUISupportQt
        VTK::RenderingAnnotation
)
