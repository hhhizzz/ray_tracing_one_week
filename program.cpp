#include <iomanip>
#include <iostream>

#include "object/camera.h"
#include "object/hittable_list.h"
#include "object/sphere.h"
#include "material/material.h"
#include "utility/color.h"
#include "utility/rtweekend.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
Color ray_color(const Ray& r, const Hittable& world, int depth) {
  HitRecord hit_record;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0) {
    return {0, 0, 0};
  }

  if (world.hit(r, 0.001f, infinity, &hit_record)) {
    Ray scattered;
    Color attenuation;

    if (hit_record.material->scatter(r, hit_record, &attenuation, &scattered)) {
      return attenuation * ray_color(scattered, world, depth - 1);
    }
    return {0, 0, 0};

  } else {
    Vec3 unit_direction = unit_vector(r.Direction());
    auto t = 0.5f * (unit_direction.Y() + 1.0f);
    return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
  }
}
#pragma clang diagnostic pop

HittableList random_scene() {
  HittableList world;

  auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
        shared_ptr<Material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Color::Random() * Color::Random();
          sphere_material = make_shared<Lambertian>(albedo);
          world.Add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Color::Random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<Metal>(albedo, fuzz);
          world.Add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<Dielectric>(1.5);
          world.Add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<Dielectric>(1.5);
  world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  return world;
}

int main() {
  // Image
  const auto aspect_ratio = 3.0 / 2.0;
  const int image_width = 1200;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 500;
  const int max_depth = 50;

  // World
  auto world = random_scene();

  // Camera
  Point3 look_from(13.0, 2.0, 3.0);
  Point3 look_at(0, 0, 0);
  Vec3 v_up(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;
  auto v_fov = 20.0;

  Camera camera(look_from, look_at, v_up, v_fov, aspect_ratio, aperture,
                dist_to_focus);

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanline's remaining:" << j << ' ' << std::flush;
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