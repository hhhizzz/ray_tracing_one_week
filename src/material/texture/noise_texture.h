#pragma once

#include "texture.h"
#include "utility/perlin.h"

class NoiseTexture : public Texture {
 public:
  NoiseTexture() = default;
  explicit NoiseTexture(double scale) : scale_(scale) {}

  [[nodiscard]] Color Value(double u, double v,
                            const Point3& p) const override {
    return Color(1, 1, 1) * 0.5 *
           (1 + sin(scale_ * p.Z() + 10 * noise_.Terb(p)));
  }

 private:
  Perlin noise_;
  double scale_{};
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_NOISETEXTURE_H
