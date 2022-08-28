#pragma once

#include "material.h"
#include "utility/rtweekend.h"

struct HitRecord;

class Material {
 public:
  virtual bool scatter(const Ray& r_in, const HitRecord& rec,
                       Color* attenuation, Ray* scattered) const = 0;
};

#include "dielectric.h"
#include "lambertian.h"
#include "metal.h"

#pragma endregion
