//
// Created by Qiwei Huang on 2022/8/31.
//

#pragma once
#include "rtweekend.h"

class Aabb {
 public:
  Aabb() = default;
  Aabb(const Point3& a, const Point3& b) {
    minimum_ = a;
    maximum_ = b;
  }

  [[nodiscard]] Point3 Minimum() const { return this->minimum_; }
  [[nodiscard]] Point3 Maximum() const { return this->maximum_; }

  [[nodiscard]] bool Hit(const Ray& r, double t_min, double t_max) const {
    for (int a = 0; a < 3; a++) {
      auto t0 = fmin((minimum_[a] - r.Origin()[a]) / r.Direction()[a],
                     (maximum_[a] - r.Origin()[a]) / r.Direction()[a]);
      auto t1 = fmax((minimum_[a] - r.Origin()[a]) / r.Direction()[a],
                     (maximum_[a] - r.Origin()[a]) / r.Direction()[a]);
      t_min = fmax(t0, t_min);
      t_max = fmin(t1, t_max);
      if (t_max <= t_min) {
        return false;
      }
    }
    return true;
  }

  [[nodiscard]] static Aabb SurroundingBox(const Aabb& box0, const Aabb& box1) {
    auto small = Point3(fmin(box0.minimum_.X(), box1.minimum_.X()),
                        fmin(box0.minimum_.Y(), box1.minimum_.Y()),
                        fmin(box0.minimum_.Z(), box1.minimum_.Z()));
    auto big = Point3(fmax(box0.maximum_.X(), box1.maximum_.X()),
                      fmax(box0.maximum_.Y(), box1.maximum_.Y()),
                      fmax(box0.maximum_.Z(), box1.maximum_.Z()));
    return {small, big};
  }

 private:
  Point3 minimum_;
  Point3 maximum_;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_AABB_H
