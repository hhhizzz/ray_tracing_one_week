#pragma once

#include "material.h"
#include "solid_color.h"
#include "texture/texture.h"

class DiffuseLight : public Material {
 public:
  explicit DiffuseLight(std::shared_ptr<Texture> a) : emit_(std::move(a)) {}
  explicit DiffuseLight(Color c) : emit_(std::make_shared<SolidColor>(c)) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, Color* attenuation,
               Ray* scattered) const override {
    return false;
  }
  Color Emitted(double u, double v, const Point3& p) const override {
    return emit_->Value(u, v, p);
  }

 private:
  std::shared_ptr<Texture> emit_;
};

#pragma endregion
