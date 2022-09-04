#pragma once

#include <utility>

#include "object/hittable.h"

class Translate : public Hittable {
 public:
  Translate(std::shared_ptr<Hittable> p, const Vec3& displacement)
      : ptr(std::move(p)), offset(displacement) {}

  bool Hit(const Ray& r, double t_min, double t_max,
           HitRecord* rec) const override;

  bool BoundingBox(double time0, double time1, Aabb* output_box) const override;

 private:
  std::shared_ptr<Hittable> ptr;
  Vec3 offset;
};

bool Translate::Hit(const Ray& r, double t_min, double t_max,
                    HitRecord* rec) const {
  Ray moved_r(r.Origin() - offset, r.Direction(), r.Time());
  if (!ptr->Hit(moved_r, t_min, t_max, rec)) {
    return false;
  }

  rec->p += offset;
  rec->SetFaceNormal(moved_r, rec->normal);

  return true;
}

bool Translate::BoundingBox(double time0, double time1,
                            Aabb* output_box) const {
  if (!ptr->BoundingBox(time0, time1, output_box)) {
    return false;
  }

  *output_box =
      Aabb(output_box->Minimum() + offset, output_box->Maximum() + offset);
  return true;
}

#pragma endregion