#pragma once

#include "utility/vec3.h"

class Ray {
 public:
  Ray() = default;
  Ray(const Point3& origin, const Vec3& direction)
      : origin_(origin), direction_(direction) {}

  [[nodiscard]] Point3 Origin() const { return this->origin_; }
  [[nodiscard]] Vec3 Direction() const { return this->direction_; }

  [[nodiscard]] Point3 At(double t) const { return origin_ + t * direction_; }

 public:
  Point3 origin_;
  Vec3 direction_;
};
#pragma endregion
