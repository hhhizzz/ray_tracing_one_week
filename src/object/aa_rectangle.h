#pragma once

#include "hittable.h"
#include "utility/rtweekend.h"

class XyRectangle : public Hittable {
 public:
  XyRectangle() = default;
  XyRectangle(double x0, double x1, double y0, double y1, double k,
              std::shared_ptr<Material> mat)
      : x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), material_(std::move(mat)) {}

  [[nodiscard]] bool Hit(const Ray& r, double t_min, double t_max,
                         HitRecord* rec) const override;

  [[nodiscard]] bool BoundingBox(double time0, double time1,
                                 Aabb* output_box) const override {
    // The bounding box must have non-zero width in each dimension, so pad the
    // Z dimension a small amount.
    *output_box =
        Aabb(Point3(x0_, y0_, k_ - 0.0001), Point3(x1_, y1_, k_ + 0.0001));
    return true;
  }

 private:
  std::shared_ptr<Material> material_;
  double x0_, x1_, y0_, y1_, k_;
};

bool XyRectangle::Hit(const Ray& r, double t_min, double t_max,
                      HitRecord* hit_record) const {
  auto t = (k_ - r.Origin().Z()) / r.Direction().Z();
  if (t < t_min || t > t_max) {
    return false;
  }
  auto x = r.Origin().X() + t * r.Direction().X();
  auto y = r.Origin().Y() + t * r.Direction().Y();
  if (x < x0_ || x > x1_ || y < y0_ || y > y1_) {
    return false;
  }
  hit_record->u = (x - x0_) / (x1_ - x0_);
  hit_record->v = (y - y0_) / (y1_ - y0_);
  hit_record->t = t;
  auto outward_normal = Vec3(0, 0, 1);
  hit_record->SetFaceNormal(r, outward_normal);
  hit_record->material = material_;
  hit_record->p = r.At(t);
  return true;
}

#pragma endregion