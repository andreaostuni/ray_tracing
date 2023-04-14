#ifndef INCLUDE_RTWEEKEND_HPP_
#define INCLUDE_RTWEEKEND_HPP_

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees)
{
  return degrees * pi / 180.0;
}

// Common Headers

#include "ray.hpp"
#include "vec3.hpp"

#endif /* INCLUDE_RTWEEKEND_HPP_ */