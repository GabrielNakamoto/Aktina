#ifndef RAYTRACEABLE_H
#define RAYTRACEABLE_H

#include <iostream>
#include "ray.h"
#include "vec3.h"

struct Hit
{
    double t;
    vec3f normal;
    vec3f point;

    bool frontFace;

    void setNormal(const Ray &r, const vec3f &outNormal)
    {
        // if they are face in the same direction this will be positive
        // meaning the ray is coming from the back of the object
        frontFace = dot(r.direction, outNormal) < 0;

        normal = frontFace ? outNormal : -outNormal;
    }
};



class RayTraceable
{
public:

    virtual ~RayTraceable() = default;

    virtual bool intersect(const Ray &r, Hit &hitInfo, float tMin, float tMax) const = 0;
};




class Sphere : public RayTraceable
{
private:

    vec3f center;
    float radius;

public:

    Sphere(const vec3f &center, float radius) : center(center), radius(std::fmax(0.0,radius))
    {

    }

    bool intersect(const Ray &r, Hit &hitInfo, float tMin, float tMax) const
    {
        vec3f oc = center - r.origin;
        auto a = dot(r.direction, r.direction);
        auto h = dot(oc, r.direction);
        auto c = dot(oc, oc) - radius*radius;

        auto discriminant = h*h - a*c;

        if(discriminant < 0) return false;

        discriminant = std::sqrt(discriminant);

        auto t = (h - discriminant) / a;

        if(t <= tMin || t >= tMax){
            // check other point
            t = (h + discriminant) / a;
            if(t <= tMin || t >= tMax)
                return false;
        }

        hitInfo.t = t;
        hitInfo.point = r.at(t);

        // this works because the vector is from the center
        // of the circle to a point on its surface, meaning
        // the length of it has to be equal to the radius
        //
        // dividing by the radius normalizes it
        vec3f outNormal = (hitInfo.point - center) / radius;

        hitInfo.setNormal(r, outNormal);

        return true;
    }
};

#endif
