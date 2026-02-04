#include "VTKAdapter.h"

#include "../core/SimulationResult.h"

#include <vtkImageData.h>
#include <vtkSmartPointer.h>

#include <limits>

vtkSmartPointer<vtkImageData>
VTKAdapter::createImageFromResult(const SimulationResult& result)
{
    const int nx = result.nx();
    const int ny = result.ny();

    // 1. First, calculate the minimum and maximum values ​​of the scalar on the CPU side.
    double minVal = std::numeric_limits<double>::max();
    double maxVal = std::numeric_limits<double>::lowest();

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            double v = result.value(i, j);
            if (v < minVal) minVal = v;
            if (v > maxVal) maxVal = v;
        }
    }

    // To prevent the case where min == max (i.e., the field is essentially a constant).
    if (maxVal - minVal < 1e-12) {
        maxVal = minVal + 1e-12;
    }

    // 2. Create a VTK image using an unsigned char scalar (0-255 grayscale).
    auto image = vtkSmartPointer<vtkImageData>::New();

    image->SetDimensions(nx, ny, 1);
    image->SetSpacing(1.0, 1.0, 1.0);
    image->SetOrigin(0.0, 0.0, 0.0);

    image->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

    // 3. Normalize the result to [0, 255] and write it to vtkImageData.
    const double invRange = 255.0 / (maxVal - minVal);

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {

            double v = result.value(i, j);
            double normalized = (v - minVal) * invRange; // 0..255

            // Cut off to legal range
            if (normalized < 0.0) normalized = 0.0;
            if (normalized > 255.0) normalized = 255.0;

            auto* ptr = static_cast<unsigned char*>(image->GetScalarPointer(i, j, 0));
            if (!ptr) {
                continue;
            }

            *ptr = static_cast<unsigned char>(normalized);
        }
    }

    image->Modified();

    return image;
}
