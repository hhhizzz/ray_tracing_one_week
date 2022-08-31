//
// Created by Qiwei Huang on 2022/8/31.
//

#pragma once

#include "rtweekend.h"

class Perlin {
 public:
  Perlin() {
    ran_float = new double[point_count_];
    for (int i = 0; i < point_count_; ++i) {
      ran_float[i] = random_double();
    }
    perm_x_ = PerlinGeneratePerm();
    perm_y_ = PerlinGeneratePerm();
    perm_z_ = PerlinGeneratePerm();
  }
  ~Perlin() {
    delete[] ran_float;
    delete[] perm_x_;
    delete[] perm_y_;
    delete[] perm_z_;
  }

  [[nodiscard]] double Noise(const Point3& p) const {
    auto u = p.X() - floor(p.X());
    auto v = p.Y() - floor(p.Y());
    auto w = p.Z() - floor(p.Z());

    auto i = static_cast<int>(floor(p.X()));
    auto j = static_cast<int>(floor(p.Y()));
    auto k = static_cast<int>(floor(p.Z()));

    double c[2][2][2];

    for (int di = 0; di < 2; di++) {
      for (int dj = 0; dj < 2; dj++) {
        for (int dk = 0; dk < 2; dk++) {
          c[di][dj][dk] =
              ran_float[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^
                        perm_z_[(k + dk) & 255]];
        }
      }
    }
    return TrilinearInterp(c, u, v, w);
  }

 private:
  static const int point_count_ = 256;
  double* ran_float;
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
      int target = random_int(0, i);
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

  static double TrilinearInterp(double c[2][2][2], double u, double v,
                                double w) {
    auto accumlate = 0.0;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          accumlate += (i * u + (1 - i) * (1 - u)) *
                       (j * v + (1 - j) * (1 - v)) *
                       (k * w + (1 - k) * (1 - w)) * c[i][j][k];
        }
      }
    }
    return accumlate;
  }
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_PERLLIN_H
