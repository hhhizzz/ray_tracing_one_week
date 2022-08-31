//
// Created by Qiwei Huang on 2022/8/31.
//

#pragma once
#include "texture.h"

class SolidColor : public Texture {
 public:
  explicit SolidColor(Color c) : color_value_(c) {}
  explicit SolidColor(double red, double green, double blue)
      : SolidColor(Color(red, green, blue)) {}

  [[nodiscard]] Color Value(double u, double v,
                            const Point3& p) const override {
    return color_value_;
  }

 private:
  Color color_value_;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_SOLID_COLOR_H
