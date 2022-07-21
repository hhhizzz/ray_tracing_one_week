#pragma once

#include "utility/rtweekend.h"

struct HitRecord;

class Material {
 public:
  virtual bool scatter(const Ray& r_in, const HitRecord& rec,
                       Color* attenuation, Ray* scattered) const = 0;
};

class Dielectric : public Material {
 public:
  explicit Dielectric(float index_of_refraction)
      : index_of_refraction_(index_of_refraction) {}

  virtual bool scatter(const Ray& r_in, const HitRecord& hitRecord,
                       Color* attenuation, Ray* scattered) const {
    *attenuation = Color(1.0, 1.0, 1.0);
    double refraction_ratio = hitRecord.front_face
                                  ? (1.0 / index_of_refraction_)
                                  : index_of_refraction_;

    Vec3 unit_direction = unit_vector(r_in.Direction());
    double cos_theta = fmin(dot(-unit_direction, hitRecord.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
    Vec3 direction;

    if (cannot_refract ||
        reflectance(cos_theta, refraction_ratio) > random_double()) {
      direction = reflect(unit_direction, hitRecord.normal);
    } else {
      direction = refract(unit_direction, hitRecord.normal, refraction_ratio);
    }

    *scattered = Ray(hitRecord.p, direction);
    return true;
  }

  float index_of_refraction_;

 private:
  static double reflectance(double cosine, double refraction_ratio) {
    double r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

class Metal : public Material {
 public:
  explicit Metal(const Color& a, double f) : albedo_(a), fuzz_(f) {}

  bool scatter(const Ray& r_in, const HitRecord& rec, Color* attenuation,
               Ray* scattered) const override {
    Vec3 reflected = reflect(unit_vector(r_in.Direction()), rec.normal);
    *scattered = Ray(rec.p, reflected + fuzz_ * random_in_unit_sphere());
    *attenuation = albedo_;
    return (dot(scattered->Direction(), rec.normal) > 0);
  }

  Color albedo_;
  double fuzz_;
};

class Lambertian : public Material {
 public:
  explicit Lambertian(const Color& a) : albedo_(a) {}

  bool scatter(const Ray& r_in, const HitRecord& hit_record, Color* attenuation,
               Ray* scattered) const override {
    auto scatter_direction = hit_record.normal + random_unit_vector();

    // Catch degenerate scatter direction
    if (scatter_direction.NearZero()) {
      scatter_direction = hit_record.normal;
    }

    *scattered = Ray(hit_record.p, scatter_direction);
    *attenuation = albedo_;
    return true;
  }

  Color albedo_;
};

#pragma endregion
