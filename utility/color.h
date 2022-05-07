#pragma once

#include <iostream>

#include "utility/vec3.h"

void write_color(std::ostream& out, Color pixel_color) {
  // Write the translated [0,255] value of each color component.ß
  out << static_cast<int>(255.999 * pixel_color.x()) << ' '
      << static_cast<int>(255.999 * pixel_color.y()) << ' '
      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#pragma endregion
