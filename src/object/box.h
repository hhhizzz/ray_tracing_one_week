#pragma once
#include "aa_rectangle.h"
#include "hittable_list.h"
#include "utility/rtweekend.h"

class Box : public Hittable {
 public:
  Box() = default;
  Box(const Point3& p0, const Point3& p1, std::shared_ptr<Material> ptr);

  [[nodiscard]] bool Hit(const Ray& r, double t_min, double t_max,
                         HitRecord* rec) const override {
    return box_.Hit(r, t_min, t_max, rec);
  }

  [[nodiscard]] bool BoundingBox(double time0, double time1,
                                 Aabb* output_box) const override {
    *output_box = Aabb(box_min_, box_max_);
    return true;
  }

 private:
  Point3 box_min_;
  Point3 box_max_;
  HittableList box_;
};

Box::Box(const Point3& p0, const Point3& p1, std::shared_ptr<Material> ptr) {
  box_min_ = p0;
  box_max_ = p1;

  box_.Add(std::make_shared<XyRectangle>(p0.X(), p1.X(), p0.Y(), p1.Y(), p1.Z(),
                                         ptr));
  box_.Add(std::make_shared<XyRectangle>(p0.X(), p1.X(), p0.Y(), p1.Y(), p0.Z(),
                                         ptr));

  box_.Add(std::make_shared<XzRectangle>(p0.X(), p1.X(), p0.Z(), p1.Z(), p1.Y(),
                                         ptr));
  box_.Add(std::make_shared<XzRectangle>(p0.X(), p1.X(), p0.Z(), p1.Z(), p0.Y(),
                                         ptr));

  box_.Add(std::make_shared<YzRectangle>(p0.Y(), p1.Y(), p0.Z(), p1.Z(), p1.X(),
                                         ptr));
  box_.Add(std::make_shared<YzRectangle>(p0.Y(), p1.Y(), p0.Z(), p1.Z(), p0.X(),
                                         ptr));
}

#pragma endregion
