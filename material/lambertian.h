//
// Created by Qiwei Huang on 2022/8/28.
//

#pragma once
#include "material.h"

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

    *scattered = Ray(hit_record.p, scatter_direction, r_in.Time());
    *attenuation = albedo_;
    return true;
  }

  Color albedo_;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_LAMBERTIAN_H
