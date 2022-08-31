//
// Created by Qiwei Huang on 2022/8/28.
//

#pragma once
#include <utility>

#include "material.h"
#include "solid_color.h"

class Lambertian : public Material {
 public:
  explicit Lambertian(const Color& a) : albedo_(make_shared<SolidColor>(a)) {}
  explicit Lambertian(std::shared_ptr<Texture> a) : albedo_(std::move(a)) {}

  bool Scatter(const Ray& r_in, const HitRecord& hit_record, Color* attenuation,
               Ray* scattered) const override {
    auto scatter_direction = hit_record.normal + RandomUnitVector();

    // Catch degenerate Scatter direction
    if (scatter_direction.NearZero()) {
      scatter_direction = hit_record.normal;
    }

    *scattered = Ray(hit_record.p, scatter_direction, r_in.Time());
    *attenuation = albedo_->Value(hit_record.u, hit_record.v, hit_record.p);
    return true;
  }

  std::shared_ptr<Texture> albedo_;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_LAMBERTIAN_H
