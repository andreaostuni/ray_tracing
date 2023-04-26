#include "rtweekend.hpp"

#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

#include <iostream>
/*
 *  The ray_color function is the heart of the ray tracer.
 *  It takes a ray as input and returns a color.
 *  The ray_color function is called recursively to generate reflections and refractions.
 *  The ray_color function is also called for each pixel in the image to generate the final image.
 */
color ray_color(const ray& r, const hittable& world, int depth)
{
  hit_record rec;

  if (depth <= 0)
    return color(0, 0, 0);  // if we've exceeded the ray bounce limit, no more light is gathered

  if (world.hit(r, 0.001, infinity, rec))
  {
    // we compute the normal vector at the point of intersection
    // the normal vector is a unit vector that points outward from the surface
    // the normal vector is computed by subtracting the center of the sphere (0,0,-1)from the point of intersection
    // r.at(t)
    // in diffuse shading, we compute the color of the point of intersection by adding a random vector to the normal
    // vector
    point3 target = rec.p + rec.normal + random_unit_vector();
    // we return a color that comes from the direction of the random vector
    return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
  }
  // we take the unit vector of the ray's direction
  vec3 unit_direction = unit_vector(r.direction());
  // we map the y coordinate of the unit vector to the range [0,1]
  auto t = 0.5 * (unit_direction.y() + 1.0);
  // we linearly interpolate between white and blue
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 100;
  const int max_depth = 50;

  // World
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  // Camera
  camera cam;

  // Render
  // P3 is the magic number for PPM
  // 256 256 is the width and height of the image
  // 255 is the maximum value of a color channel
  std::cout << "P3\n " << image_width << " " << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j)
  {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i)
    {
      color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s)
      {
        // the u goes from 0 to 1 from left to right
        auto u = (i + random_double()) / (image_width - 1);
        // the v goes from 0 to 1 from bottom to top
        auto v = (j + random_double()) / (image_height - 1);
        // the ray r is casted from the camera origin to the projection plane
        ray r = cam.get_ray(u, v);
        // we add the color of the ray to the pixel color
        pixel_color += ray_color(r, world, max_depth);
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }
  std::cerr << "file written" << std::endl;
  return 0;
}