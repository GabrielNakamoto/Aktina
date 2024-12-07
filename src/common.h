#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <optional>
#include <cstdlib>
#include <memory>
#include <cmath>

#include "vec3.h"

using std::shared_ptr;
using std::make_shared;

const float infinity = std::numeric_limits<float>::infinity();
const double pi = 3.1415926535897932385;

inline float toRadians(float degrees)
{
    return degrees * pi / 180.0;
}

inline float randomFloat()
{
    return std::rand() / (RAND_MAX + 1.0);
}

inline float randomFloat(float min, float max)
{
    return min + (max - min)*randomFloat();
}

inline vec3f randomUnitVector()
{
    while (true)
    {
        auto p = vec3f(randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1));
        auto ls = dot(p,p);
        if(1e-36 < ls && ls <= 1)
            return p.normalize();
    }
}

inline vec3f randomOnHemisphere(const vec3f &normal)
{
    auto v = randomUnitVector();
    return dot(v, normal) > 0 ? v : -v;
}


#endif
