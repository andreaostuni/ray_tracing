#ifndef INCLUDE_HITTABLE_LIST_HPP_
#define INCLUDE_HITTABLE_LIST_HPP_

#include "hittable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;

class hittable_list : public hittable
{
public:
  hittable_list()
  {
  }
  hittable_list(shared_ptr<hittable> object)
  {
    add(object);
  }

  void clear()
  {
    objects.clear();
  }
  void add(shared_ptr<hittable> object)
  {
    objects.push_back(object);
  }

  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
  std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
  // temp_rec is a local variable that is used to store the hit_record of the object that is hit
  hit_record temp_rec;
  // hit_anything is a local variable that is used to store whether or not the ray hits any object
  bool hit_anything = false;
  // closest_so_far is a local variable that is used to store the distance from the ray origin to the first point of
  // intersection
  auto closest_so_far = t_max;

  for (const auto& object : objects)
  {
    // if the ray hits the object in the range [t_min, closest_so_far]
    if (object->hit(r, t_min, closest_so_far, temp_rec))
    {
      // set hit_anything to true
      hit_anything = true;
      // set closest_so_far to the distance from the ray origin to the new point of intersection
      closest_so_far = temp_rec.t;
      // set rec to temp_rec
      rec = temp_rec;
    }
  }
  return hit_anything;
}

#endif /* INCLUDE_HITTABLE_LIST_HPP_ */
