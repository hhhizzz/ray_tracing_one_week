#include <iostream>

#include "utility/color.h"
#include "utility/ray.h"
#include "utility/vec3.h"

float hit_sphere(const Point3& center, double radius, const Ray& r) {
  auto oc = r.Origin() - center;
  auto a = r.Direction().LengthSquared();
  auto b = 2.0f * dot(oc, r.Direction());
  auto c = oc.LengthSquared() - radius * radius;
  auto discriminant = b * b - 4 * a * c;
  if (discriminant >= 0) {
    return (-b - sqrt(discriminant)) / (2 * a);
  } else {
    return -1.0f;
  }
}

Color ray_color(const Ray& r) {
  auto t = hit_sphere(Point3(0, 0, -1), 0.5, r);
  if (t > 0) {
    auto N = unit_vector(r.At(t) - Point3(0, 0, -1));
    return 0.5 * Color(N.X() + 1, N.Y() + 1, N.Z() + 1);
  }
  Vec3 unit_direction = unit_vector(r.Direction());
  t = 0.5 * (unit_direction.Y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;

  auto origin = Point3(0, 0, 0);
  auto horizontal = Vec3(viewport_width, 0, 0);
  auto vertical = Vec3(0, viewport_height, 0);
  auto lower_left_corner =
      origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining:" << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      auto u = static_cast<double>(i) / (image_width - 1);
      auto v = static_cast<double>(j) / (image_height - 1);

      if (i == image_width / 2 && j == 168) {
        std::cerr << "u: " << u << " v: " << v << std::endl;
      }

      auto pixel = lower_left_corner + u * horizontal + v * vertical;
      Ray ray(origin, pixel - origin);
      Color pixel_color = ray_color(ray);
      write_color(std::cout, pixel_color);
    }
  }
  std::cerr << "\nDone.\n";
}
