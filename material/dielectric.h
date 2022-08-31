//
// Created by Qiwei Huang on 2022/8/28.
//

#pragma once
#include "material.h"

class Dielectric : public Material {
 public:
  explicit Dielectric(float index_of_refraction)
      : index_of_refraction_(index_of_refraction) {}

  bool scatter(const Ray& r_in, const HitRecord& hitRecord, Color* attenuation,
               Ray* scattered) const override {
    *attenuation = Color(1.0, 1.0, 1.0);
    double refraction_ratio = hitRecord.front_face
                                  ? (1.0 / index_of_refraction_)
                                  : index_of_refraction_;

    Vec3 unit_direction = UnitVector(r_in.Direction());
    double cos_theta = fmin(Dot(-unit_direction, hitRecord.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
    Vec3 direction;

    if (cannot_refract ||
        reflectance(cos_theta, refraction_ratio) > random_double()) {
      direction = Reflect(unit_direction, hitRecord.normal);
    } else {
      direction = Refract(unit_direction, hitRecord.normal, refraction_ratio);
    }

    *scattered = Ray(hitRecord.p, direction, r_in.Time());
    return true;
  }

  float index_of_refraction_;

 private:
  static double reflectance(double cosine, double refraction_ratio) {
    double r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_DIELECTRIC_H
