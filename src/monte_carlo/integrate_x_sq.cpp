#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "utility/rtweekend.h"

class PDF {
 public:
  [[nodiscard]] virtual double Value(double x) const = 0;
  [[nodiscard]] virtual double Generate() const = 0;
};

class AvgPDF : public PDF {
 public:
  [[nodiscard]] double Value(double x) const override { return 0.5f; }
  [[nodiscard]] double Generate() const override { return RandomDouble(0, 2); }
};

class ProportionalPDF : public PDF {
 public:
  [[nodiscard]] double Value(double x) const override { return x / 2; }
  [[nodiscard]] double Generate() const override {
    return 2 * std::sqrt(RandomDouble());
  }
};

class PerfectPDF : public PDF {
 public:
  [[nodiscard]] double Value(double x) const override { return x * x * 3 / 8; }
  [[nodiscard]] double Generate() const override {
    return std::pow(RandomDouble(0, 8), 1. / 3.);
  }
};

double mc(PDF const& p, int N) {
  double sum = 0.0;
  for (int i = 0; i < N; i++) {
    auto x = p.Generate();
    sum += x * x / p.Value(x);
  }
  return sum / N;
}

void output(const std::string& name, const double result) {
  double standard = 8.0 / 3;

  std::cout << std::setprecision(12);
  std::cout << name << ": " << std::endl;
  std::cout << "\tresult = " << result << std::endl;
  std::cout << std::setprecision(4);
  std::cout << "\tdeviation: " << std::abs(result - standard) / standard * 100
            << "%" << std::endl;
}

int main() {
  int N = 10000;

  std::cout << std::fixed;

  output("pdf: y = 0.5", mc(AvgPDF(), N));
  output("pdf: y = x / 2", mc(ProportionalPDF(), N));
  output("pdf: y = x^2 * 8 / 3", mc(PerfectPDF(), N));
}