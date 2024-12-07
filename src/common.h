#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <memory>
#include <cmath>

using std::shared_ptr;
using std::make_shared;

const float infinity = std::numeric_limits<float>::infinity();
const double pi = 3.1415926535897932385;

inline float toRadians(float degrees)
{
    return degrees * pi / 180.0;
}

#include "vec3.h"
#include "ray.h"

#endif
