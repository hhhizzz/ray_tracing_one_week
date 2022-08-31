#pragma once

#include <memory>

#include "utility/aabb.h"
#include "utility/rtweekend.h"

class Material;

struct HitRecord {
  Point3 p;
  Vec3 normal;
  std::shared_ptr<Material> material;
  double t;
  double u;
  double v;
  bool front_face;

  void SetFaceNormal(const Ray& ray, const Vec3& outward_normal) {
    front_face = dot(ray.Direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
 public:
  virtual bool Hit(const Ray& r, double t_min, double t_max,
                   HitRecord* rec) const = 0;
  virtual bool BoundingBox(double time0, double time1,
                           Aabb* output_box) const = 0;
};
#include "material/material.h"

#pragma endregion
