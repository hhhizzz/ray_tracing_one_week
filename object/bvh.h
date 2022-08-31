//
// Created by Qiwei Huang on 2022/8/31.
//

#pragma once
#include <algorithm>

#include "hittable.h"
#include "hittable_list.h"
#include "utility/rtweekend.h"

class BvhNode : public Hittable {
 public:
  BvhNode() = default;
  BvhNode(HittableList& list, double time0, double time1)
      : BvhNode(list.objects_, 0, static_cast<long>(list.objects_.size()),
                time0, time1) {}
  BvhNode(std::vector<std::shared_ptr<Hittable>>& src_objects, long start,
          long end, double time0, double time1);

  bool Hit(const Ray& r, double t_min, double t_max,
           HitRecord* hit_record) const override;
  bool BoundingBox(double time0, double time1, Aabb* output_box) const override;

  static bool BoxCompare(const std::shared_ptr<Hittable>& a,
                         const std::shared_ptr<Hittable>& b, int axis) {
    Aabb box_a;
    Aabb box_b;
    if (!a->BoundingBox(0, 0, &box_a) || !b->BoundingBox(0, 0, &box_b)) {
      std::cerr << "No bounding box in BvhNode constructor.\n";
    }
    return box_a.Minimum().e_[axis] < box_b.Minimum().e_[axis];
  }
  static bool BoxXCompare(const std::shared_ptr<Hittable>& a,
                          const std::shared_ptr<Hittable>& b) {
    return BoxCompare(a, b, 0);
  }
  static bool BoxYCompare(const std::shared_ptr<Hittable>& a,
                          const std::shared_ptr<Hittable>& b) {
    return BoxCompare(a, b, 1);
  }
  static bool BoxZCompare(const std::shared_ptr<Hittable>& a,
                          const std::shared_ptr<Hittable>& b) {
    return BoxCompare(a, b, 2);
  }

 public:
  std::shared_ptr<Hittable> left_;
  std::shared_ptr<Hittable> right_;
  Aabb box_;
};

bool BvhNode::Hit(const Ray& r, double t_min, double t_max,
                  HitRecord* hit_record) const {
  if (!box_.Hit(r, t_min, t_max)) {
    return false;
  }

  auto hit_left = left_->Hit(r, t_min, t_max, hit_record);
  auto hit_right =
      right_->Hit(r, t_min, hit_left ? hit_record->t : t_max, hit_record);
  return hit_left || hit_right;
}

bool BvhNode::BoundingBox(double time0, double time1, Aabb* output_box) const {
  *output_box = box_;
  return true;
}

BvhNode::BvhNode(std::vector<std::shared_ptr<Hittable>>& src_objects,
                 long start, long end, double time0, double time1) {
  int axis = RandomInt(0, 2);
  auto comparator = (axis == 0)   ? BoxXCompare
                    : (axis == 1) ? BoxYCompare
                                  : BoxZCompare;
  auto object_span = end - start;

  if (object_span == 1) {
    left_ = right_ = src_objects[start];
  } else if (object_span == 2) {
    if (comparator(src_objects[start], src_objects[start + 1])) {
      left_ = src_objects[start];
      right_ = src_objects[start + 1];
    } else {
      left_ = src_objects[start + 1];
      right_ = src_objects[start];
    }
  } else {
    std::sort(src_objects.begin() + start, src_objects.begin() + end,
              comparator);
    auto mid = start + object_span / 2;
    left_ = std::make_shared<BvhNode>(src_objects, start, mid, time0, time1);
    right_ = std::make_shared<BvhNode>(src_objects, mid, end, time0, time1);
  }

  Aabb box_left, box_right;

  if (!left_->BoundingBox(time0, time1, &box_left) ||
      !right_->BoundingBox(time0, time1, &box_right)) {
    std::cerr << "No bounding box in BvhNode constructor.\n";
  }

  box_ = Aabb::SurroundingBox(box_left, box_right);
}
#pragma endregion  // RAY_TRACING_ONE_WEEK_BVH_H
