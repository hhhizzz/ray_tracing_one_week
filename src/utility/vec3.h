#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3 {
 public:
  Vec3() : e_{0, 0, 0} {}
  Vec3(double e0, double e1, double e2) : e_{e0, e1, e2} {}

  static Vec3 Random() {
    return {RandomDouble(), RandomDouble(), RandomDouble()};
  }

  static Vec3 Random(double min, double max) {
    return {RandomDouble(min, max), RandomDouble(min, max),
            RandomDouble(min, max)};
  }

  [[nodiscard]] double X() const { return e_[0]; }
  [[nodiscard]] double Y() const { return e_[1]; }
  [[nodiscard]] double Z() const { return e_[2]; }
  [[nodiscard]] bool NearZero() const {
    return (e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2]) < 1e-8;
  }

  Vec3 operator-() const { return {-e_[0], -e_[1], -e_[2]}; }
  double operator[](int i) const { return e_[i]; }
  double& operator[](int i) { return e_[i]; }

  Vec3& operator+=(const Vec3& v) {
    e_[0] += v.e_[0];
    e_[1] += v.e_[1];
    e_[2] += v.e_[2];
    return *this;
  }

  Vec3& operator*=(const double t) {
    e_[0] *= t;
    e_[1] *= t;
    e_[2] *= t;
    return *this;
  }

  Vec3& operator/=(const double t) { return *this *= 1 / t; }

  [[nodiscard]] double LengthSquared() const {
    return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
  }

  [[nodiscard]] double Length() const { return sqrt(this->LengthSquared()); }

 public:
  double e_[3];
};

using Point3 = Vec3;
using Color = Vec3;

// Vec3 Utility Functions
Vec3 RandomInUnitSphere() {
  while (true) {
    auto p = Vec3::Random(-1, 1);
    if (p.LengthSquared() >= 1) continue;
    return p;
  }
}

Vec3 RandomInUnitDisk() {
  while (true) {
    auto p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
    if (p.LengthSquared() >= 1) continue;
    return p;
  }
}

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
  return out << v.e_[0] << ' ' << v.e_[1] << ' ' << v.e_[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
  return {u.e_[0] + v.e_[0], u.e_[1] + v.e_[1], u.e_[2] + v.e_[2]};
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
  return {u.e_[0] - v.e_[0], u.e_[1] - v.e_[1], u.e_[2] - v.e_[2]};
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
  return {u.e_[0] * v.e_[0], u.e_[1] * v.e_[1], u.e_[2] * v.e_[2]};
}

inline Vec3 operator*(double t, const Vec3& v) {
  return {t * v.e_[0], t * v.e_[1], t * v.e_[2]};
}

inline Vec3 operator*(const Vec3& v, double t) { return t * v; }

inline Vec3 operator/(Vec3 v, double t) { return 1 / t * v; }

inline double Dot(const Vec3& u, const Vec3& v) {
  return u.e_[0] * v.e_[0] + u.e_[1] * v.e_[1] + u.e_[2] * v.e_[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
  return {u.e_[1] * v.e_[2] - u.e_[2] * v.e_[1],
          u.e_[2] * v.e_[0] - u.e_[0] * v.e_[2],
          u.e_[0] * v.e_[1] - u.e_[1] * v.e_[0]};
}

inline Vec3 UnitVector(Vec3 v) { return v / v.Length(); }

Vec3 RandomUnitVector() { return UnitVector(RandomInUnitSphere()); }

Vec3 Reflect(const Vec3& v, const Vec3& n) { return v - 2 * Dot(v, n) * n; }

Vec3 Refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
  auto cos_theta = fmin(Dot(-uv, n), 1.0f);
  Vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
  Vec3 r_out_perp = -sqrt(fabs(1.0f - r_out_parallel.LengthSquared())) * n;
  return r_out_parallel + r_out_perp;
}

#pragma endregion
