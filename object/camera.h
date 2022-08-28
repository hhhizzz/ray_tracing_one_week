#pragma once

#include "utility/rtweekend.h"

class Camera {
 public:
  Camera(Point3 look_from, Point3 look_at, Vec3 v_up, double v_fov,
         double aspect_ratio, double aperture, double focus_dist,
         double time0 = 0, double time1 = 0) {
    auto theta = degrees_to_radians(v_fov);
    auto height = tan(theta / 2);
    auto viewport_height = 2.0 * height;
    auto viewport_width = aspect_ratio * viewport_height;

    w_ = unit_vector(look_from - look_at);
    u_ = unit_vector(cross(v_up, w_));
    v_ = cross(w_, u_);

    origin_ = look_from;
    horizontal_ = focus_dist * viewport_width * u_;
    vertical_ = focus_dist * viewport_height * v_;
    lower_left_corner_ =
        origin_ - horizontal_ / 2 - vertical_ / 2 - focus_dist * w_;

    lens_radius_ = aperture / 2;
    time0_ = time0;
    time1_ = time1;
  }

  [[nodiscard]] Ray GetRay(double s, double t) const {
    Vec3 rd = lens_radius_ * random_in_unit_disk();
    Vec3 offset = u_ * rd.X() + v_ * rd.Y();

    return {
        origin_ + offset,
        lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset,
        random_double(time0_, time1_)};
  }

 private:
  Point3 origin_;
  Point3 lower_left_corner_;
  Vec3 horizontal_;
  Vec3 vertical_;
  Vec3 u_, v_, w_;
  double lens_radius_;
  double time0_, time1_;  // shutter open/close times
};

#pragma endregion
