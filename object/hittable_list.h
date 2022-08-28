#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
 public:
  HittableList() = default;
  explicit HittableList(const shared_ptr<Hittable>& object) { Add(object); }
  void Add(const shared_ptr<Hittable>& object) { objects_.push_back(object); }
  bool Hit(const Ray& r, double t_min, double t_max,
           HitRecord* hit_record) const override;
  std::vector<shared_ptr<Hittable>> objects_;
};

bool HittableList::Hit(const Ray& r, double t_min, double t_max,
                       HitRecord* hit_record) const {
  HitRecord temp_record;
  bool hit_anything = false;
  double closest_so_far = t_max;
  for (const auto& object : objects_) {
    if (object->Hit(r, t_min, closest_so_far, &temp_record)) {
      hit_anything = true;
      closest_so_far = temp_record.t;
      *hit_record = temp_record;
    }
  }
  return hit_anything;
}

#pragma endregion
