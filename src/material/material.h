#pragma once

#include "utility/rtweekend.h"

struct HitRecord;

class Material {
 public:
  virtual Color Emitted(double u, double v, const Point3& p) const {
    return {0, 0, 0};
  }
  virtual bool Scatter(const Ray& r_in, const HitRecord& rec,
                       Color* attenuation, Ray* scattered) const = 0;
};

#pragma endregion
