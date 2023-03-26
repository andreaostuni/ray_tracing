#ifndef INCLUDE_COLOR_HPP_
#define INCLUDE_COLOR_HPP_

#include "vec3.hpp"
#include <iostream>

void write_color(std::ostream& out, color pixel_color)
{
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(255.999 * pixel_color.x()) << ' ' << static_cast<int>(255.999 * pixel_color.y()) << ' '
      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif /* INCLUDE_COLOR_HPP_ */
