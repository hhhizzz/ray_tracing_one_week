#pragma once

#include "utility/rtweekend.h"

class Camera {
 public:
  Camera(Point3 look_from, Point3 look_at, Vec3 v_up, double v_fov,
         double aspect_ratio, double aperture, double focus_dist,
         Color background = Color(0, 0, 0), double time0 = 0,
         double time1 = 0) {
    look_from_ = look_from;
    look_at_ = look_at;
    v_fov_ = v_fov;
    v_up_ = v_up;
    aspect_ratio_ = aspect_ratio;
    aperture_ = aperture;
    background_ = background;
    focus_dist_ = focus_dist;

    auto theta = DegreesToRadians(v_fov);
    auto height = tan(theta / 2);
    auto viewport_height = 2.0 * height;
    auto viewport_width = aspect_ratio * viewport_height;

    w_ = UnitVector(look_from - look_at);
    u_ = UnitVector(cross(v_up, w_));
    v_ = cross(w_, u_);

    origin_ = look_from;
    horizontal_ = focus_dist * viewport_width * u_;
    vertical_ = focus_dist * viewport_height * v_;
    lower_left_corner_ =
        origin_ - horizontal_ / 2 - vertical_ / 2 - focus_dist * w_;

    lens_radius_ = aperture / 2;
    background_ = background;
    time0_ = time0;
    time1_ = time1;
  }

  [[nodiscard]] Ray GetRay(double s, double t) const {
    Vec3 rd = lens_radius_ * RandomInUnitDisk();
    Vec3 offset = u_ * rd.X() + v_ * rd.Y();

    return {
        origin_ + offset,
        lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset,
        RandomDouble(time0_, time1_)};
  }

 public:
  Point3 origin_;
  double aspect_ratio_;
  Point3 look_from_;
  Point3 look_at_;
  Color background_;
  double v_fov_;
  Point3 v_up_;
  double aperture_;
  double focus_dist_;
  double time0_, time1_;  // shutter open/close times

 private:
  Point3 lower_left_corner_;
  Vec3 horizontal_;
  Vec3 vertical_;
  Vec3 u_, v_, w_;
  double lens_radius_;
};

#pragma endregion
