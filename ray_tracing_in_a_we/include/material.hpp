#ifndef INCLUDE_MATERIAL_HPP_
#define INCLUDE_MATERIAL_HPP_

#include "rtweekend.hpp"

struct hit_record;

class material
{
public:
  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
  lambertian(const color& a) : albedo(a)
  {
  }

  // scatter the ray in a random direction
  // the direction is chosen from a unit sphere
  // the unit sphere is centered at the point of intersection
  // the radius of the unit sphere is 1
  // the point of intersection is the center of the u
  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
  {
    auto scatter_direction = rec.normal + random_unit_vector();
    // catch degenerate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

  color albedo;
};

class metal : public material
{
public:
  metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1)
  {
  }

  // scatter the ray in the direction of the reflection
  // the direction of the reflection is calculated using the formula
  // r = v - 2 * dot(v, n) * n
  // where v is the direction of the ray and n is the normal vector at the point of intersection
  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
  {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    // fuzzy reflection
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }

  color albedo;
  double fuzz;
};

class dielectric : public material
{
public:
  dielectric(double index_of_refraction) : ir(index_of_refraction)
  {
  }
  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
  {
    attenuation = color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
    {
      direction = reflect(unit_direction, rec.normal);
    }
    else
    {
      direction = refract(unit_direction, rec.normal, refraction_ratio);
    }
    scattered = ray(rec.p, direction);
    return true;
  }

private:
  static double reflectance(double cosine, double ref_idx)
  {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }

public:
  double ir;  // Index of Refraction
};

#endif /* INCLUDE_MATERIAL_HPP_ */
