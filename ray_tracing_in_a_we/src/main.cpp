#include <iostream>
#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"

/*
 *  The ray_color function is the heart of the ray tracer.
 *  It takes a ray as input and returns a color.
 *  The ray_color function is called recursively to generate reflections and refractions.
 *  The ray_color function is also called for each pixel in the image to generate the final image.
 */
bool hit_sphere(const point3& center, double radius, const ray& r)
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
  return (discriminant > 0);
}
color ray_color(const ray& r)
{
  // if the ray hits the sphere, we return a red color
  if (hit_sphere(point3(0, 0, -1), 0.5, r))
  {
    return color(1, 0, 0);
  }
  // we take the unit vector of the ray's direction
  vec3 unit_direction = unit_vector(r.direction());
  // we map the y coordinate of the unit vector to the range [0,1]
  auto t = 0.5 * (unit_direction.y() + 1.0);
  // we linearly interpolate between white and light blue
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

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
      color pixel_color = ray_color(r);
      write_color(std::cout, pixel_color);
    }
  }
  std::cerr << "file written" << std::endl;
  return 0;
}