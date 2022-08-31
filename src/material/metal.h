//
// Created by Qiwei Huang on 2022/8/28.
//

#pragma once
#include "material.h"

class Metal : public Material {
 public:
  explicit Metal(const Color& a, double f) : albedo_(a), fuzz_(f) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, Color* attenuation,
               Ray* scattered) const override {
    Vec3 reflected = Reflect(UnitVector(r_in.Direction()), rec.normal);
    *scattered =
        Ray(rec.p, reflected + fuzz_ * RandomInUnitSphere(), r_in.Time());
    *attenuation = albedo_;
    return (Dot(scattered->Direction(), rec.normal) > 0);
  }

  Color albedo_;
  double fuzz_;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_METAL_H
