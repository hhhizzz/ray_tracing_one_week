#include <iostream>

#include "utility/color.h"
#include "utility/hittable_list.h"
#include "utility/sphere.h"
#include "utility/rtweekend.h"

Color ray_color(const Ray& r, const Hittable& world) {
  HitRecord hit_record;
  if (world.hit(r, 0.001f, infinity, &hit_record)) {
    return 0.5f * (hit_record.normal + Color(1, 1, 1));
  } else {
    Vec3 unit_direction = unit_vector(r.Direction());
    auto t = 0.5f * (unit_direction.Y() + 1.0f);
    return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
  }
}

int main() {
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  // World
  HittableList world;
  world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

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
      Color pixel_color = ray_color(ray, world);
      write_color(std::cout, pixel_color);
    }
  }
  std::cerr << "\nDone.\n";
}
