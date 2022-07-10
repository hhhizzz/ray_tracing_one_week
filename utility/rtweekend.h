#pragma once

#include <cmath>
#include <limits>
#include <memory>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358979323846;

// Utility functions

double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

// Common Headers

#include "utility/ray.h"
#include "utility/vec3.h"

#pragma endregion
