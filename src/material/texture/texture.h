#pragma once

#include "utility/rtweekend.h"

class Texture {
 public:
  [[nodiscard]] virtual Color Value(double u, double v,
                                    const Point3& p) const = 0;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_TEXTURE_H
