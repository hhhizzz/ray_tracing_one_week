#pragma once

#include "utility/vec3.h"

class Ray {
 public:
  Ray() {}
  Ray(const Point3& origin, const Vec3& direction)
      : origin_(origin), direction_(direction) {}

  Point3 Origin() const { return this->origin_; }
  Vec3 Direction() const { return this->direction_; }

  Point3 At(double t) const { return origin_ + t * direction_; }

 public:
  Point3 origin_;
  Vec3 direction_;
};
#pragma endregion
