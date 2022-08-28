#pragma once

#include <iostream>

#include "utility/rtweekend.h"
#include "utility/vec3.h"

void write_color(std::ostream& out, Color pixel_color, int samples_per_pixel) {
  auto r = pixel_color.X();
  auto g = pixel_color.Y();
  auto b = pixel_color.Z();

  // Divide the color by the number of samples and gamma-correct for gamma=2.0.
  auto scale = 1.0 / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

  // Write the translated [0,255] value of each color component.ß
  out << static_cast<int>(256 * clamp(r, 0.0f, 0.999f)) << ' '
      << static_cast<int>(256 * clamp(g, 0.0f, 0.999f)) << ' '
      << static_cast<int>(256 * clamp(b, 0.0f, 0.999f)) << '\n';
}

#pragma endregion
