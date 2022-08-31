#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

#include "material/diffuse_light.h"
#include "material/solid_color.h"
#include "material/texture/checker_texture.h"
#include "material/texture/image_texture.h"
#include "material/texture/noise_texture.h"
#include "object/aa_rectangle.h"
#include "object/bvh.h"
#include "object/camera.h"
#include "object/hittable_list.h"
#include "object/moving_sphere.h"
#include "object/sphere.h"
#include "utility/color.h"
#include "utility/rtweekend.h"

Color RayColor(const Ray& r, const Color& background, const Hittable& world,
               int depth) {
  HitRecord hit_record;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0) {
    return {0, 0, 0};
  }
  // If the ray hits nothing, return the background color.
  if (!world.Hit(r, 0.001, infinity, &hit_record)) {
    return background;
  }

  Ray scattered;
  Color attenuation;
  Color emitted =
      hit_record.material->Emitted(hit_record.u, hit_record.v, hit_record.p);

  if (!hit_record.material->Scatter(r, hit_record, &attenuation, &scattered)) {
    return emitted;
  }

  return emitted +
         attenuation * RayColor(scattered, background, world, depth - 1);
}

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
      auto choose_mat = RandomDouble();
      Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

      if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
        shared_ptr<Material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Color::Random() * Color::Random();
          sphere_material = make_shared<Lambertian>(albedo);
          auto center2 = center + Vec3(0, RandomDouble(0, 0.5), 0);
          boxes.Add(make_shared<MovingSphere>(center, center2, time0, time1,
                                              0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Color::Random(0.5, 1);
          auto fuzz = RandomDouble(0, 0.5);
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

HittableList SampleLight(std::shared_ptr<Camera>& camera) {
  HittableList objects;
  auto per_text = make_shared<NoiseTexture>(4);
  objects.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000,
                                  make_shared<Lambertian>(per_text)));
  objects.Add(make_shared<Sphere>(Point3(0, 2, 0), 2,
                                  make_shared<Lambertian>(per_text)));

  auto diff_light = make_shared<DiffuseLight>(make_shared<SolidColor>(4, 4, 4));
  objects.Add(make_shared<Sphere>(Point3(0, 7, 0), 2, diff_light));
  objects.Add(make_shared<XyRectangle>(3, 5, 1, 3, -2, diff_light));

  Point3 look_from(26.0, 3.0, 6.0);
  Point3 look_at(0, 2, 0);
  auto aperture = 0.01;

  camera =
      make_shared<Camera>(look_from, look_at, camera->v_up_, camera->v_fov_,
                          camera->aspect_ratio_, aperture, camera->focus_dist_);

  return objects;
}

int main(int argc, char** argv) {
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  int image_width = 1600;
  int image_height = static_cast<int>(image_width / aspect_ratio);
  int samples_per_pixel = 500;
  const int max_depth = 50;
  std::string scene_name = "Random";

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

  // Camera
  Point3 look_from(13.0, 2.0, 3.0);
  Point3 look_at(0, 0, 0);
  Vec3 v_up(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;
  auto v_fov = 20.0;

  auto camera = std::make_shared<Camera>(look_from, look_at, v_up, v_fov,
                                         aspect_ratio, aperture, dist_to_focus,
                                         Color(0.70, 0.80, 1.00), 0.0f, 1.0f);

  // World
  auto world_map = std::map<std::string, HittableList>{
      {"Random", RandomScene(false, false)},
      {"WithTime", RandomScene(true, false)},
      {"CheckerTexture", RandomScene(true, true)},
      {"TwoSpheres", TwoSpheres()},
      {"TwoPerlinSpheres", TwoPerlinSpheres()},
      {"Earth", Earth()},
      {"SampleLight", SampleLight(camera)},
  };

  if (world_map.find(scene_name) == world_map.end()) {
    std::cerr << "Scene " << scene_name << " not found" << std::endl;
    return 1;
  }
  std::cerr << "Rendering Scene:  " << scene_name << std::endl;
  auto world = world_map[scene_name];

  // Output
  std::ofstream ofs(scene_name + ".ppm");

  // Render
  ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanline's remaining:" << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      Color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        auto u = (i + RandomDouble()) / (image_width - 1);
        auto v = (j + RandomDouble()) / (image_height - 1);

        Ray ray = camera->GetRay(u, v);
        auto background = camera->background_;
        pixel_color += RayColor(ray, background, world, max_depth);
      }
      write_color(ofs, pixel_color, samples_per_pixel);
    }
  }
  std::cerr << "\nDone.\n";
}
