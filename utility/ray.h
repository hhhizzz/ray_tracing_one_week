#pragma once

#include "utility/vec3.h"

class Ray {
 public:
  Ray() = default;
  Ray(const Point3& origin, const Vec3& direction, double time)
      : origin_(origin), direction_(direction), time_(time) {}

  [[nodiscard]] Point3 Origin() const { return this->origin_; }
  [[nodiscard]] Vec3 Direction() const { return this->direction_; }
  [[nodiscard]] double Time() const { return this->time_; }

  [[nodiscard]] Point3 At(double t) const {
    return this->origin_ + t * this->direction_;
  }

 public:
  Point3 origin_;
  Vec3 direction_;
  double time_;
};
#pragma endregion
