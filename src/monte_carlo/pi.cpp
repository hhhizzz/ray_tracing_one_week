#include <iomanip>
#include <iostream>

#include "utility/rtweekend.h"

int main() {
  int inside_circle = 0;
  int inside_circle_stratified = 0;
  int sqrt_N = 10000;
  for (int i = 0; i < sqrt_N; i++) {
    for (int j = 0; j < sqrt_N; j++) {
      auto x = RandomDouble(-1, 1);
      auto y = RandomDouble(-1, 1);
      if (x * x + y * y < 1) {
        inside_circle++;
      }
      x = 2 * (i + RandomDouble()) / static_cast<double>(sqrt_N) - 1;
      y = 2 * (j + RandomDouble()) / static_cast<double>(sqrt_N) - 1;
      if (x * x + y * y < 1) {
        inside_circle_stratified++;
      }
    }
  }

  auto N = static_cast<double>(sqrt_N * sqrt_N);
  std::cout << std::fixed;
  auto regular = 4 * inside_circle / N;
  std::cout << "Regular Monte Carlo: " << std::endl
            << std::setprecision(12) << "\tValue: " << 4 * inside_circle / N
            << std::endl
            << std::setprecision(6)
            << "\tdeviation: " << std::abs(regular - pi) / pi * 100 << "%"
            << std::endl;

  auto stratified = 4 * inside_circle_stratified / N;
  std::cout << "Stratified Monte Carlo: " << std::endl
            << std::setprecision(12) << "\tValue: " << stratified << std::endl
            << std::setprecision(6)
            << "\tdeviation: " << std::abs(stratified - pi) / pi * 100 << "%"
            << std::endl;
}