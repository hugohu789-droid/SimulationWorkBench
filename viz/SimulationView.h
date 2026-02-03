#ifndef SIMULATIONVIEW_H
#define SIMULATIONVIEW_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkImageActor.h>

class SimulationResult;
class vtkOrientationMarkerWidget;
class vtkAxesActor;

class SimulationView : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationView(QWidget *parent = nullptr);

    void setResult(const SimulationResult& result);

private:
    void setupAxesWidget();

private:
    QVTKOpenGLNativeWidget* m_vtkWidget = nullptr;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkImageActor> m_actor;

    vtkSmartPointer<vtkAxesActor> m_axesActor;
    vtkSmartPointer<vtkOrientationMarkerWidget> m_axesWidget;
signals:
};

#endif // SIMULATIONVIEW_H
