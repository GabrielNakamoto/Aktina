#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "vec3.h"

struct Ray
{
    vec3f origin, direction;

    Ray()
    {

    }

    Ray(const vec3f &origin, const vec3f &direction) : origin(origin), direction(direction)
    {

    }

    vec3f at(float t) const
    {
        return origin + t * direction;
    }
};

#endif

