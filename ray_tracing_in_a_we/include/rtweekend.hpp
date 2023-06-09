#ifndef INCLUDE_RTWEEKEND_HPP_
#define INCLUDE_RTWEEKEND_HPP_

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double random_double()
{
  // Returns a random real in [0,1).
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double random_double(double min, double max)
{
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

inline double degrees_to_radians(double degrees)
{
  return degrees * pi / 180.0;
}

inline double clamp(double x, double min, double max)
{
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

// Common Headers

#include "ray.hpp"
#include "vec3.hpp"

#endif /* INCLUDE_RTWEEKEND_HPP_ */
