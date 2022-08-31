//
// Created by Qiwei Huang on 2022/8/31.
//

#pragma once

#include "texture.h"
#include "utility/perlin.h"

class NoiseTexture : public Texture {
 public:
  NoiseTexture() = default;

  [[nodiscard]] Color Value(double u, double v,
                            const Point3& p) const override {
    return Color(1, 1, 1) * noise_.Noise(p);
  }

 private:
  Perlin noise_;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_NOISETEXTURE_H
