#include <iostream>

#include "utility/camera.h"
#include "utility/color.h"
#include "utility/hittable_list.h"
#include "utility/material.h"
#include "utility/rtweekend.h"
#include "utility/sphere.h"

Color ray_color(const Ray& r, const Hittable& world, int depth) {
  HitRecord hit_record;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0) {
    return Color(0, 0, 0);
  }

  if (world.hit(r, 0.001f, infinity, &hit_record)) {
    Ray scattered;
    Color attenuation;

    if (hit_record.material->scatter(r, hit_record, &attenuation, &scattered)) {
      return attenuation * ray_color(scattered, world, depth - 1);
    }
    return Color(0, 0, 0);

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
  const int samples_per_pixel = 1000;
  const int max_depth = 100;

  // World
  HittableList world;
  auto material_gound = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
  auto material_left = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
  auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

  world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, material_gound));
  world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5, material_center));
  world.Add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5, material_left));
  world.Add(make_shared<Sphere>(Point3(1, 0, -1), 0.5, material_right));

  // Camera
  Camera camera;

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining:" << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      Color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        auto u = (i + random_double()) / (image_width - 1);
        auto v = (j + random_double()) / (image_height - 1);

        Ray ray = camera.GetRay(u, v);
        pixel_color += ray_color(ray, world, max_depth);
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }
  std::cerr << "\nDone.\n";
}
