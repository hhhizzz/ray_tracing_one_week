#pragma once
#include <utility>

#include "material.h"
#include "solid_color.h"

class Isotropic : public Material {
 public:
  explicit Isotropic(const Color& a)
      : albedo_(std::make_shared<SolidColor>(a)) {}
  explicit Isotropic(std::shared_ptr<Texture> a) : albedo_(std::move(a)) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, Color* attenuation,
               Ray* scattered) const override {
    *scattered = Ray(rec.p, RandomInUnitSphere(), r_in.Time());
    *attenuation = albedo_->Value(rec.u, rec.v, rec.p);
    return true;
  }

 private:
  std::shared_ptr<Texture> albedo_;
};

#pragma endregion