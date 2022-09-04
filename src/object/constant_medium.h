#pragma once

#include <utility>

#include "material/isotropic.h"
#include "object/hittable.h"

class ConstantMedium : public Hittable {
 public:
  ConstantMedium(std::shared_ptr<Hittable> b, double d,
                 std::shared_ptr<Texture> a)
      : boundary(std::move(b)),
        neg_inv_density(-1 / d),
        phase_function(std::make_shared<Isotropic>(a)) {}

  ConstantMedium(std::shared_ptr<Hittable> b, double d, Color c)
      : boundary(std::move(b)),
        neg_inv_density(-1 / d),
        phase_function(std::make_shared<Isotropic>(c)) {}

  bool Hit(const Ray& r, double t_min, double t_max,
                   HitRecord* rec) const override;

  bool BoundingBox(double time0, double time1,
                           Aabb* output_box) const override {
    return boundary->BoundingBox(time0, time1, output_box);
  }

 public:
  std::shared_ptr<Hittable> boundary;
  std::shared_ptr<Material> phase_function;
  double neg_inv_density;
};

bool ConstantMedium::Hit(const Ray& r, double t_min, double t_max,
                         HitRecord* rec) const {
  // Print occasional samples when debugging. To enable, set enableDebug true
  // and rebuild.
  constexpr bool enableDebug = false;
  const bool debugging = enableDebug && RandomDouble() < 0.00001;

  HitRecord rec1, rec2;

  if (!boundary->Hit(r, -infinity, infinity, &rec1)) return false;

  if (!boundary->Hit(r, rec1.t + 0.0001, infinity, &rec2)) return false;

  if (debugging)
    std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

  if (rec1.t < t_min) rec1.t = t_min;
  if (rec2.t > t_max) rec2.t = t_max;

  if (rec1.t >= rec2.t) {
    return false;
  }

  if (rec1.t < 0) {
    rec1.t = 0;
  }

  const auto ray_length = r.direction_.Length();
  const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
  const auto hit_distance = neg_inv_density * std::log(RandomDouble());

  if (hit_distance > distance_inside_boundary) return false;

  rec->t = rec1.t + hit_distance / ray_length;
  rec->p = r.At(rec->t);

  if (debugging) {
    std::cerr << "hit_distance = " << hit_distance << '\n'
              << "rec.t = " << rec->t << '\n'
              << "rec.p = " << rec->p << '\n';
  }

  rec->normal = {1, 0, 0};  // arbitrary
  rec->front_face = true;   // also arbitrary
  rec->material = phase_function;

  return true;
}

#pragma endregion