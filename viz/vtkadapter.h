#ifndef VTKADAPTER_H
#define VTKADAPTER_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

class SimulationResult;

class VTKAdapter
{
public:
    VTKAdapter();

public:
    // Returns a smart pointer to vtkImageData
    static vtkSmartPointer<vtkImageData> createImageFromResult(const SimulationResult& result);
};

#endif // VTKADAPTER_H
