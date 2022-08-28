#pragma once

#include <memory>
#include <utility>

#include "utility/hittable.h"
#include "utility/vec3.h"

class Sphere : public Hittable {
 public:
  Sphere() {
    center_ = Point3(0, 0, 0);
    radius_ = 1.0;
  }
  Sphere(const Point3& center, double radius, shared_ptr<Material> material)
      : center_(center), radius_(radius), material_(std::move(material)) {}
  bool hit(const Ray& r, double t_min, double t_max,
           HitRecord* hit_record) const override;

  Point3 center_;
  double radius_;
  std::shared_ptr<Material> material_;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max,
                 HitRecord* hit_record) const {
  auto oc = r.Origin() - center_;
  auto a = r.Direction().LengthSquared();
  auto b = 2.0f * dot(oc, r.Direction());
  auto c = oc.LengthSquared() - radius_ * radius_;
  auto discriminant = b * b - 4 * a * c;
  if (discriminant >= 0) {
    auto sqrt_d = sqrt(discriminant);

    auto root = (-b - sqrt_d) / (2 * a);
    if (root < t_min || root > t_max) {
      root = (-b + sqrt_d) / (2 * a);
      if (root < t_min || root > t_max) {
        return false;
      }
    }
    hit_record->t = root;
    hit_record->p = r.At(root);
    auto outward_normal = (hit_record->p - center_) / radius_;
    hit_record->SetFaceNormal(r, outward_normal);
    hit_record->material = material_;

    return true;
  }
  return false;
}

#pragma endregion
