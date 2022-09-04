#pragma once

#include "rtweekend.h"

class Perlin {
 public:
  Perlin() {
    ran_vec_ = new Vec3[point_count_];
    for (int i = 0; i < point_count_; ++i) {
      ran_vec_[i] = UnitVector(Vec3::Random(-1, 1));
    }
    perm_x_ = PerlinGeneratePerm();
    perm_y_ = PerlinGeneratePerm();
    perm_z_ = PerlinGeneratePerm();
  }
  ~Perlin() {
    delete[] ran_vec_;
    delete[] perm_x_;
    delete[] perm_y_;
    delete[] perm_z_;
  }

  [[nodiscard]] double Terb(const Point3& p, int depth = 7) const {
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; ++i) {
      accum += weight * Noise(temp_p);
      weight *= 0.5;
      temp_p *= 2;
    }

    return fabs(accum);
  }

  [[nodiscard]] double Noise(const Point3& p) const {
    auto u = p.X() - floor(p.X());
    auto v = p.Y() - floor(p.Y());
    auto w = p.Z() - floor(p.Z());

    auto i = static_cast<int>(floor(p.X()));
    auto j = static_cast<int>(floor(p.Y()));
    auto k = static_cast<int>(floor(p.Z()));

    Vec3 c[2][2][2];

    for (int di = 0; di < 2; di++) {
      for (int dj = 0; dj < 2; dj++) {
        for (int dk = 0; dk < 2; dk++) {
          c[di][dj][dk] =
              ran_vec_[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^
                       perm_z_[(k + dk) & 255]];
        }
      }
    }
    return TrilinearInterp(c, u, v, w);
  }

 private:
  static const int point_count_ = 256;
  Vec3* ran_vec_;
  int* perm_x_;
  int* perm_y_;
  int* perm_z_;

  static int* PerlinGeneratePerm() {
    auto p = new int[point_count_];
    for (int i = 0; i < Perlin::point_count_; ++i) {
      p[i] = i;
    }
    Permute(p, point_count_);
    return p;
  }

  static void Permute(int* p, int n) {
    for (int i = n - 1; i > 0; --i) {
      int target = RandomInt(0, i);
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

  static double TrilinearInterp(Vec3 c[2][2][2], double u, double v, double w) {
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accumulate = 0.0;

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          Vec3 weight_v(u - i, v - j, w - k);
          accumulate +=
              (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) *
              (k * ww + (1 - k) * (1 - ww)) * Dot(c[i][j][k], weight_v);
        }
      }
    }
    return accumulate;
  }
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_PERLLIN_H
