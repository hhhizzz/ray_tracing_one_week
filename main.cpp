#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

#include "object/bvh.h"
#include "object/camera.h"
#include "object/hittable_list.h"
#include "object/moving_sphere.h"
#include "object/sphere.h"
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

  if (world.Hit(r, 0.001f, infinity, &hit_record)) {
    Ray scattered;
    Color attenuation;

    if (hit_record.material->scatter(r, hit_record, &attenuation, &scattered)) {
      return attenuation * ray_color(scattered, world, depth - 1);
    }
    return {0, 0, 0};

  } else {
    Vec3 unit_direction = UnitVector(r.Direction());
    auto t = 0.5f * (unit_direction.Y() + 1.0f);
    return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
  }
}
#pragma clang diagnostic pop

HittableList RandomScene(bool has_time = true,
                         bool has_checker_texture = true) {
  HittableList boxes;
  HittableList world;

  double time0 = 0.0f;
  double time1 = 1.0f;
  if (!has_time) {
    time0 = 0.0f;
    time1 = 0.0f;
  }

  if (has_checker_texture) {
    auto checker =
        make_shared<CheckTexture>(make_shared<SolidColor>(0.2f, 0.3f, 0.1f),
                                  make_shared<SolidColor>(0.9f, 0.9f, 0.9f));
    boxes.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000,
                                  make_shared<Lambertian>(checker)));
  } else {
    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    boxes.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));
  }

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
          auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
          boxes.Add(make_shared<MovingSphere>(center, center2, time0, time1,
                                              0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Color::Random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<Metal>(albedo, fuzz);
          boxes.Add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<Dielectric>(1.5);
          boxes.Add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<Dielectric>(1.5);
  boxes.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  boxes.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  boxes.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  world.Add(make_shared<BvhNode>(boxes, 0, 1));
  return world;
}

HittableList TwoSpheres() {
  HittableList objects;

  auto checker =
      make_shared<CheckTexture>(make_shared<SolidColor>(0.2f, 0.3f, 0.1f),
                                make_shared<SolidColor>(0.9f, 0.9f, 0.9f));
  objects.Add(make_shared<Sphere>(Point3(0, -10, 0), 10,
                                  make_shared<Lambertian>(checker)));
  objects.Add(make_shared<Sphere>(Point3(0, 10, 0), 10,
                                  make_shared<Lambertian>(checker)));
  return objects;
}

HittableList TwoPerlinSpheres() {
  HittableList objects;

  auto per_text = make_shared<NoiseTexture>(4);
  objects.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000,
                                  make_shared<Lambertian>(per_text)));
  objects.Add(make_shared<Sphere>(Point3(0, 2, 0), 2,
                                  make_shared<Lambertian>(per_text)));
  return objects;
}

HittableList Earth() {
  auto earth_texture = make_shared<ImageTexture>("resources/earth-map.jpg");
  auto earth_surface = make_shared<Lambertian>(earth_texture);
  auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);
  return HittableList(globe);
}

int main(int argc, char** argv) {
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  int image_width = 1600;
  int image_height = static_cast<int>(image_width / aspect_ratio);
  int samples_per_pixel = 500;
  const int max_depth = 50;
  std::string scene_name = "random";

  // Read Environment Variables
  if (const char* env_p = std::getenv("IMAGE_WIDTH")) {
    image_width = std::stoi(env_p);
    image_height = static_cast<int>(image_width / aspect_ratio);
  }
  if (const char* env_p = std::getenv("SPP")) {
    samples_per_pixel = std::stoi(env_p);
  }
  if (const char* env_p = std::getenv("SCENE")) {
    scene_name = env_p;
  }

  // World
  auto world_map = std::map<std::string, HittableList>{
      {"Random", RandomScene(false, false)},
      {"WithTime", RandomScene(true, false)},
      {"CheckerTexture", RandomScene(true, true)},
      {"TwoSpheres", TwoSpheres()},
      {"TwoPerlinSpheres", TwoPerlinSpheres()},
      {"Earth", Earth()},
  };

  if (world_map.find(scene_name) == world_map.end()) {
    std::cerr << "Scene " << scene_name << " not found" << std::endl;
    return 1;
  }
  std::cerr << "Rendering Scene:  " << scene_name << std::endl;
  auto world = world_map[scene_name];

  // Camera
  Point3 look_from(13.0, 2.0, 3.0);
  Point3 look_at(0, 0, 0);
  Vec3 v_up(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;
  auto v_fov = 20.0;

  if (const char* env_p = std::getenv("APERTURE")) {
    aperture = std::stod(env_p);
  }

  Camera camera(look_from, look_at, v_up, v_fov, aspect_ratio, aperture,
                dist_to_focus, 0.0f, 1.0f);

  // Output
  std::ofstream ofs(scene_name + ".ppm");

  // Render
  ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

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
      write_color(ofs, pixel_color, samples_per_pixel);
    }
  }
  std::cerr << "\nDone.\n";
}
