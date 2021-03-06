#include <iostream>

#include "utility/color.h"
#include "utility/vec3.h"

int main() {
  // Image

  const int image_width = 256;
  const int image_height = 256;

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining:" << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      Color pixel_color(static_cast<double>(i) / (image_width - 1),
                        static_cast<double>(j) / (image_height - 1), 0.5f);
      write_color(std::cout, pixel_color);
    }
  }
  std::cerr << "\nDone.\n";
}
