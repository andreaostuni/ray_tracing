#ifndef INCLUDE_CAMERA_HPP_
#define INCLUDE_CAMERA_HPP_

#include "rtweekend.hpp"

class camera
{
public:
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

  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist)
  {
    // vfov is vertical field of view in degrees
    // aspect_ratio is the ratio of the width to the height of the image
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);  // h is the height of the projection plane
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;  // 16/9 * 2 = 16/9 * 2.0
    w = unit_vector(lookfrom - lookat);                    // w is the direction of the camera
    u = unit_vector(cross(vup, w));                        // u is the horizontal vector
    v = cross(w, u);                                       // v is the vertical vector

    // auto focal_length = 1.0;                               // distance between the projection plane and the camera

    origin = lookfrom;                             // camera position
    horizontal = focus_dist * viewport_width * u;  // horizontal vector (x axis)
    vertical = focus_dist * viewport_height * v;   // vertical vector (y axis)
    lower_left_corner =
        origin - horizontal / 2 - vertical / 2 - focus_dist * w;  // lower left corner of the projection plane
    lens_radius = aperture / 2;
  }

  ray get_ray(double s, double t) const
  {
    vec3 rd = lens_radius * random_in_unit_disk();  // random offset from the origin (on the lens)
    vec3 offset = u * rd.x() + v * rd.y();          // offset from the origin (on the lens)
    return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
  }

private:
  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  double lens_radius;
};

#endif /* INCLUDE_CAMERA_HPP_ */
