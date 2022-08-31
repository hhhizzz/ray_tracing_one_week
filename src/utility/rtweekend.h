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

// double RandomDouble() { return rand() / (RAND_MAX + 1.0); }

double RandomDouble() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator(std::random_device{}());
  return distribution(generator);
}

double RandomDouble(double min, double max) {
  return min + (max - min) * RandomDouble();
}

inline int RandomInt(int min, int max) {
  return static_cast<int>(RandomDouble(min, max + 1));
}

double DegreesToRadians(double degrees) { return degrees * pi / 180; }

double Clamp(double x, double min, double max) {
  return x < min ? min : (x > max ? max : x);
}

// Common Headers

#include "utility/ray.h"
#include "utility/vec3.h"

#pragma endregion
