#pragma once

#include "utility/ray.h"

struct HitRecord {
  Point3 p;
  Vec3 normal;
  double t;
  bool front_face;

  void SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
    front_face = dot(ray.Direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
 public:
  virtual bool hit(const Ray& r, double t_min, double t_max,
                   HitRecord* rec) const = 0;
};

#pragma endregion
