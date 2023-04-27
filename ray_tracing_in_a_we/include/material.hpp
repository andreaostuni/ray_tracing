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

#endif /* INCLUDE_MATERIAL_HPP_ */
