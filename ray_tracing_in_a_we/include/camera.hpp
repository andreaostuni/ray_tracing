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

  camera()
  {
    // vfov is top to bottom in degrees
    auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;  // 16/9 * 2 = 16/9 * 2.0
    auto focal_length = 1.0;                               // distance between the projection plane and the camera

    origin = point3(0, 0, 0);                 // camera position
    horizontal = vec3(viewport_width, 0, 0);  // horizontal vector (x axis)
    vertical = vec3(0, viewport_height, 0);   // vertical vector (y axis)
    // The lower left corner of the projection plane is at (-2, -1, -1)
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
  }

  ray get_ray(double u, double v) const
  {
    return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
  }

private:
  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};

#endif /* INCLUDE_CAMERA_HPP_ */
