#include "rtweekend.hpp"

#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

#include <iostream>
/*
 *  The ray_color function is the heart of the ray tracer.
 *  It takes a ray as input and returns a color.
 *  The ray_color function is called recursively to generate reflections and refractions.
 *  The ray_color function is also called for each pixel in the image to generate the final image.
 */
color ray_color(const ray& r, const hittable& world)
{
  hit_record rec;
  // if the ray hits the sphere, we return a red color
  if (world.hit(r, 0, infinity, rec))
  {
    // we compute the normal vector at the point of intersection
    // the normal vector is a unit vector that points outward from the surface
    // the normal vector is computed by subtracting the center of the sphere (0,0,-1)from the point of intersection
    // r.at(t)
    return 0.5 * (rec.normal + color(1, 1, 1));
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

  // World
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  // Camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;  // 16/9 * 2 = 16/9 * 2.0
  auto focal_length = 1.0;                               // distance between the projection plane and the camera

  // The camera is a virtual camera.
  // The projection plane is a virtual plane.

  // (-2, 1,-1)  (0, 1, -1)  (2, 1, -1)
  // |----------------------|
  // |                      |
  // |         Y            |
  // |         ^            |
  // |         |            |
  // |         |            |
  // |         |            |
  // |         |            |
  //           -----------------> X
  //          /
  //         /
  //        /
  //       /
  //      Z

  // Camera origin is at (0, 0, 0)
  auto origin = point3(0, 0, 0);                 // camera position
  auto horizontal = vec3(viewport_width, 0, 0);  // horizontal vector (x axis)
  auto vertical = vec3(0, viewport_height, 0);   // vertical vector (y axis)
  // The lower left corner of the projection plane is at (-2, -1, -1)
  auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

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
      // the u goes from 0 to 1 from left to right
      auto u = double(i) / (image_width - 1);
      // the v goes from 0 to 1 from bottom to top
      auto v = double(j) / (image_height - 1);
      // the ray r is casted from the camera origin to the projection plane
      ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
      color pixel_color = ray_color(r, world);
      write_color(std::cout, pixel_color);
    }
  }
  std::cerr << "file written" << std::endl;
  return 0;
}