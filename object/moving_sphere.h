//
// Created by Qiwei Huang on 2022/8/29.
//
#pragma once

#include <utility>

#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"

class MovingSphere : public Hittable {
 public:
  MovingSphere() = default;
  MovingSphere(const Point3& center0, const Point3& center1, double time0,
               double time1, double radius, std::shared_ptr<Material> mat)
      : center0_(center0),
        center1_(center1),
        time0_(time0),
        time1_(time1),
        radius_(radius),
        material_(std::move(mat)) {}

  bool Hit(const Ray& r, double t_min, double t_max,
           HitRecord* hit_record) const override;

  [[nodiscard]] Point3 Center(double time) const;

 public:
  Point3 center0_;
  Point3 center1_;
  double time0_{};
  double time1_{};
  double radius_{};
  std::shared_ptr<Material> material_;
};

Point3 MovingSphere::Center(double time) const {
  return center0_ +
         ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}

bool MovingSphere::Hit(const Ray& r, double t_min, double t_max,
                       HitRecord* hit_record) const {
  auto oc = r.Origin() - this->Center(r.Time());
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
    auto outward_normal = (hit_record->p - this->Center(r.Time())) / radius_;
    hit_record->SetFaceNormal(r, outward_normal);
    hit_record->material = material_;

    return true;
  }
  return false;
}

#pragma endregion
