#ifndef INCLUDE_SPHERE_HPP_
#define INCLUDE_SPHERE_HPP_

#include "hittable.hpp"

class sphere : public hittable
{
public:
  sphere();
  sphere(point3 cen, double r) : center(cen), radius(r){};

  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
  point3 center;
  double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
  // the ray is defined by the equation r(t) = A + t * B
  // where A is the origin of the ray and B is the direction of the ray

  // the sphere is defined by the equation (x - c_x)^2 + (y - c_y)^2 + (z - c_z)^2 = r^2
  // where (c_x, c_y, c_z) is the center of the sphere and r is the radius of the sphere

  // we substitute the ray equation into the sphere equation
  // (A + t * B - c)^2 = r^2
  // we expand the square
  // A^2 + 2 * t * A * B + t^2 * B^2 - 2 * A * c - 2 * t * B * c + c^2 - r^2 = 0
  // we factor out the t^2

  // t^2 * B^2 + 2 * t * (A * B - B * c) + A^2 - 2 * A * c + c^2 - r^2 = 0

  // A - c is called the oc vector
  vec3 oc = r.origin() - center;

  // B^2 is called the a coefficient
  auto a = r.direction().length_squared();

  // -B * (A - c) is called the half_b coefficient
  auto half_b = dot(oc, r.direction());

  // (A - c)^2 - r^2 is called the c coefficient
  auto c = oc.length_squared() - radius * radius;

  // (b/2)^2 - ac is called the discriminant
  auto discriminant = half_b * half_b - a * c;
  // if the discriminant is negative, the ray misses the sphere
  // if the discriminant is zero, the ray hits the sphere tangentially
  // if the discriminant is positive, the ray hits the sphere at two points
  if (discriminant < 0)
    return false;
  // we return the smallest positive root
  auto sqrtd = sqrt(discriminant);

  // find the nearest root that lies in the acceptable range
  auto root = (-half_b - sqrtd) / a;

  if (root < t_min || t_max < root)
  {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
    {
      return false;
    }
  }

  // the point of intersection is rec
  rec.t = root;
  rec.p = r.at(rec.t);
  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);

  return true;
}

#endif /* INCLUDE_SPHERE_HPP_ */
