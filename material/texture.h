//
// Created by Qiwei Huang on 2022/8/31.
//

#pragma once

#include "utility/rtweekend.h"

class Texture {
 public:
  virtual Color Value(double u, double v, const Point3& p) const = 0;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_TEXTURE_H
