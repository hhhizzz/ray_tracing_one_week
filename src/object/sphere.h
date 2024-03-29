#pragma once

#include <memory>
#include <utility>

#include "hittable.h"
#include "utility/vec3.h"

class Sphere : public Hittable {
 public:
  Sphere() {
    center_ = Point3(0, 0, 0);
    radius_ = 1.0;
  }
  Sphere(const Point3& center, double radius, shared_ptr<Material> material)
      : center_(center), radius_(radius), material_(std::move(material)) {}
  bool Hit(const Ray& r, double t_min, double t_max,
           HitRecord* hit_record) const override;
  bool BoundingBox(double time0, double time1, Aabb* output_box) const override;

  Point3 center_;
  double radius_;
  std::shared_ptr<Material> material_;

 private:
  static void GetSphereUV(const Point3& p, double* u, double* v) {
    auto theta = acos(-p.Y());
    auto phi = atan2(-p.Z(), p.X()) + pi;

    *u = phi / (2 * pi);
    *v = theta / pi;
  }
};

bool Sphere::Hit(const Ray& r, double t_min, double t_max,
                 HitRecord* hit_record) const {
  auto oc = r.Origin() - center_;
  auto a = r.Direction().LengthSquared();
  auto b = 2.0f * Dot(oc, r.Direction());
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
    GetSphereUV(outward_normal, &hit_record->u, &hit_record->v);
    hit_record->material = material_;

    return true;
  }
  return false;
}

bool Sphere::BoundingBox(double time0, double time1, Aabb* output_box) const {
  *output_box = Aabb(center_ - Vec3(radius_, radius_, radius_),
                     center_ + Vec3(radius_, radius_, radius_));
  return true;
}

#pragma endregion
