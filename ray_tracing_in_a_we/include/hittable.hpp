#ifndef INCLUDE_HITTABLE_HPP_
#define INCLUDE_HITTABLE_HPP_

#include "ray.hpp"
#include "rtweekend.hpp"

struct material;

struct hit_record
{
  // the point of intersection
  point3 p;
  // the normal vector at the point of intersection
  vec3 normal;
  // the material of the object that was hit
  shared_ptr<material> mat_ptr;
  // the distance from the ray origin to the point of intersection
  double t;
  bool front_face;

  inline void set_face_normal(const ray& r, const vec3& outward_normal)
  {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable
{
public:
  // the hit function returns true if the ray hits the object
  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif /* INCLUDE_HITTABLE_HPP_ */
