#pragma once

#include <iostream>

#include "utility/vec3.h"
#include "utility/rtweekend.h"

void write_color(std::ostream& out, Color pixel_color) {
  auto r = pixel_color.X();
  auto g = pixel_color.Y();
  auto b = pixel_color.Z();

  r = sqrt(r);
  g = sqrt(g);
  b = sqrt(b);

  // Write the translated [0,255] value of each color component.ß
  out << static_cast<int>(256 * clamp(r, 0.0f, 0.999f)) << ' '
      << static_cast<int>(256 * clamp(g, 0.0f, 0.999f)) << ' '
      << static_cast<int>(256 * clamp(b, 0.0f, 0.999f)) << '\n';
}

#pragma endregion
