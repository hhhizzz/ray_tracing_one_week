#pragma once

#include "utility/rtweekend.h"

struct HitRecord;

class Material {
 public:
  virtual bool scatter(const Ray& r_in, const HitRecord& rec,
                       Color* attenuation, Ray* scattered) const = 0;
};

class Lambertian : public Material {
 public:
  explicit Lambertian(const Color& a) : albedo_(a) {}

  bool scatter(const Ray& r_in, const HitRecord& hit_record, Color* attenuation,
               Ray* scattered) const override {
    auto scatter_direction = hit_record.normal + random_unit_vector();

    // Catch degenerate scatter direction
    if (scatter_direction.NearZero()) {
      scatter_direction = hit_record.normal;
    }

    *scattered = Ray(hit_record.p, scatter_direction);
    *attenuation = albedo_;
    return true;
  }

  Color albedo_;
};

#pragma endregion
