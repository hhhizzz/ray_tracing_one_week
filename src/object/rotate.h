#pragma once

#include "object/hittable.h"

class RotateY : public Hittable {
 public:
  RotateY(std::shared_ptr<Hittable> p, double angle);

  bool Hit(const Ray& r, double t_min, double t_max,
                   HitRecord* rec) const override;

  bool BoundingBox(double time0, double time1,
                           Aabb* output_box) const override {
    *output_box = bbox_;
    return has_box_;
  }

 public:
  std::shared_ptr<Hittable> ptr_;
  double sin_theta_;
  double cos_theta_;
  bool has_box_;
  Aabb bbox_;
};

RotateY::RotateY(std::shared_ptr<Hittable> p, double angle) : ptr_(p) {
  ptr_ = p;
  auto radians = DegreesToRadians(angle);
  sin_theta_ = std::sin(radians);
  cos_theta_ = std::cos(radians);
  has_box_ = ptr_->BoundingBox(0, 1, &bbox_);

  Point3 min(infinity, infinity, infinity);
  Point3 max(-infinity, -infinity, -infinity);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        auto x = i * bbox_.Maximum().X() + (1 - i) * bbox_.Minimum().X();
        auto y = j * bbox_.Maximum().Y() + (1 - j) * bbox_.Minimum().Y();
        auto z = k * bbox_.Maximum().Z() + (1 - k) * bbox_.Minimum().Z();

        auto newX = cos_theta_ * x + sin_theta_ * z;
        auto newZ = -sin_theta_ * x + cos_theta_ * z;

        Vec3 tester(newX, y, newZ);

        for (int c = 0; c < 3; c++) {
          min[c] = fmin(min[c], tester[c]);
          max[c] = fmax(max[c], tester[c]);
        }
      }
    }
  }
  bbox_ = Aabb(min, max);
}

bool RotateY::Hit(const Ray& r, double t_min, double t_max,
                  HitRecord* rec) const {
  auto origin = r.Origin();
  auto direction = r.Direction();

  origin[0] = cos_theta_ * r.Origin()[0] - sin_theta_ * r.Origin()[2];
  origin[2] = sin_theta_ * r.Origin()[0] + cos_theta_ * r.Origin()[2];

  direction[0] = cos_theta_ * r.Direction()[0] - sin_theta_ * r.Direction()[2];
  direction[2] = sin_theta_ * r.Direction()[0] + cos_theta_ * r.Direction()[2];

  Ray rotated_r(origin, direction, r.Time());

  if (!ptr_->Hit(rotated_r, t_min, t_max, rec)) {
    return false;
  }

  auto p = rec->p;
  auto normal = rec->normal;

  p[0] = cos_theta_ * rec->p[0] + sin_theta_ * rec->p[2];
  p[2] = -sin_theta_ * rec->p[0] + cos_theta_ * rec->p[2];

  normal[0] = cos_theta_ * rec->normal[0] + sin_theta_ * rec->normal[2];
  normal[2] = -sin_theta_ * rec->normal[0] + cos_theta_ * rec->normal[2];

  rec->p = p;
  rec->SetFaceNormal(rotated_r, normal);

  return true;
}

#pragma endregion