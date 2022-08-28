#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

// Using

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358979323846;

// Utility functions

// double random_double() { return rand() / (RAND_MAX + 1.0); }

double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator(std::random_device{}());
  return distribution(generator);
}

double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

double degrees_to_radians(double degrees) { return degrees * pi / 180; }

double clamp(double x, double min, double max) {
  return x < min ? min : (x > max ? max : x);
}

// Common Headers

#include "utility/ray.h"
#include "utility/vec3.h"

#pragma endregion
