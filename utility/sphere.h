#pragma once

#include "utility/hittable.h"
#include "utility/vec3.h"

class Sphere : public Hittable {
 public:
  Sphere() {}
  Sphere(const Point3& center, double radius)
      : center_(center), radius_(radius) {}
  bool hit(const Ray& r, double t_min, double t_max,
           HitRecord* rec) const override;

  Point3 center_;
  double radius_;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max,
                 HitRecord* hit_record) const {
  auto oc = r.Origin() - center_;
  auto a = r.Direction().LengthSquared();
  auto b = 2.0f * dot(oc, r.Direction());
  auto c = oc.LengthSquared() - radius_ * radius_;
  auto discriminant = b * b - 4 * a * c;
  if (discriminant >= 0) {
    auto sqrtd = sqrt(discriminant);

    auto root = (-b - sqrtd) / (2 * a);
    if (root < t_min || root > t_max) {
      root = (-b + sqrtd) / (2 * a);
      if (root < t_min || root > t_max) {
        return false;
      }
    }
    hit_record->t = root;
    hit_record->p = r.At(root);
    auto outward_normal = (hit_record->p - center_) / radius_;
    hit_record->SetFaceNormal(r, outward_normal);
    return true;
  }
  return false;
}

#pragma endregion
