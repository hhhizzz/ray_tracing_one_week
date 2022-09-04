#pragma once

#include "material/solid_color.h"

class CheckTexture : public Texture {
 public:
  CheckTexture() = default;
  CheckTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1)
      : even_(t0), odd_(t1) {}
  CheckTexture(Color c1, Color c2)
      : even_(make_shared<SolidColor>(c1)), odd_(make_shared<SolidColor>(c2)) {}

  [[nodiscard]] Color Value(double u, double v,
                            const Point3& p) const override {
    auto sines = sin(10 * p.X()) * sin(10 * p.Y()) * sin(10 * p.Z());
    if (sines < 0) {
      return odd_->Value(u, v, p);
    } else {
      return even_->Value(u, v, p);
    }
  }

 public:
  std::shared_ptr<Texture> odd_;
  std::shared_ptr<Texture> even_;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_CHECKER_TEXTURE_H
