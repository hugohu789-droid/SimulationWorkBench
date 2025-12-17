#include "simulationview.h"
#include "vtkadapter.h"
#include "../core/SimulationResult.h"

#include <QVBoxLayout>

#include <QVTKOpenGLNativeWidget.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

SimulationView::SimulationView(QWidget* parent)
    : QWidget(parent)
{
    // Qt Layout： Fill the entire SimulationView with QVTK controls.
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_vtkWidget = new QVTKOpenGLNativeWidget(this);
    layout->addWidget(m_vtkWidget);

    // Create a VTK rendering window and hand it over to the QVTK control for management.
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    m_vtkWidget->setRenderWindow(renderWindow);

    // Create a Renderer and add it to the rendering window.
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetBackground(0.10, 0.10, 0.10);
    m_renderer->SetBackground2(0.18, 0.18, 0.22);
    m_renderer->SetGradientBackground(true);

    renderWindow->AddRenderer(m_renderer);

    // Create an empty image actor beforehand, and then feed it data when settingResult.
    m_actor = vtkSmartPointer<vtkImageActor>::New();
    m_renderer->AddActor(m_actor);

    // Parallel projection ensures the image is not distorted.
    m_renderer->GetActiveCamera()->ParallelProjectionOn();

    // Create coordinate axes
    setupAxesWidget();
}

void SimulationView::setupAxesWidget()
{
    // coordinate actor
    m_axesActor = vtkSmartPointer<vtkAxesActor>::New();
    m_axesActor->SetTotalLength(1.0, 1.0, 1.0);

    // Direction markers Widget
    m_axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    m_axesWidget->SetOrientationMarker(m_axesActor);

    // Place it in the bottom right corner (small viewport of 0.8~1.0, 0.0~0.2).
    m_axesWidget->SetViewport(0.80, 0.0, 1.0, 0.20);

    // Bind to the current interactor
    auto* rw = m_vtkWidget->renderWindow();
    auto* iren = rw->GetInteractor();
    m_axesWidget->SetInteractor(iren);

    m_axesWidget->SetEnabled(1);
    m_axesWidget->SetInteractive(0); // It cannot be dragged with the mouse; it is only for indication.
}

void SimulationView::setResult(const SimulationResult& result)
{
    if (!m_vtkWidget || !m_vtkWidget->renderWindow() || !m_renderer) {
        return;
    }

    // 1. Convert the business layer results into vtkImageData
    vtkSmartPointer<vtkImageData> image = VTKAdapter::createImageFromResult(result);

    // 2.Give the data to the image actor
    m_actor->SetInputData(image);

    // 3. Adjust the camera according to the data range (to display the complete image).
    m_renderer->ResetCamera();
    m_renderer->GetActiveCamera()->Zoom(1.0);

    // 4. Request to refresh rendering
    m_vtkWidget->renderWindow()->Render();
}
