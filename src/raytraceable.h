#ifndef RAYTRACEABLE_H
#define RAYTRACEABLE_H

#include <iostream>

#include "common.h"
#include "vec3.h"
#include "ray.h"

class Material;

class RayTraceable
{
protected:

    shared_ptr<Material> material;

public:

    struct Hit
    {
        bool hit;
        double t;
        vec3f normal;
        vec3f point;

        shared_ptr<Material> material;

        bool frontFace;

        void setNormal(const Ray &r, const vec3f &outNormal)
        {
            // if they are face in the same direction this will be positive
            // meaning the ray is coming from the back of the object
            frontFace = dot(r.direction, outNormal) < 0;

            normal = frontFace ? outNormal : -outNormal;
        }
    };

    RayTraceable(shared_ptr<Material> material) : material(material)
    {
    }

    virtual ~RayTraceable() = default;

    [[nodiscard]] virtual std::optional<Hit> intersect(const Ray &r, float tMin, float tMax) const = 0;
};




class Sphere : public RayTraceable
{
private:

    vec3f center;
    float radius;

public:

    Sphere(const vec3f &center, float radius, shared_ptr<Material> material) :
        center(center), radius(std::fmax(0.0,radius)), RayTraceable(material)
    {

    }

    [[nodiscard]] std::optional<Hit> intersect(const Ray &r, float tMin, float tMax) const override
    {
        Hit hitInfo;

        vec3f oc = center - r.origin;
        auto a = dot(r.direction, r.direction);
        auto h = dot(oc, r.direction);
        auto c = dot(oc, oc) - radius*radius;

        auto discriminant = h*h - a*c;

        if(discriminant < 0) return std::nullopt;

        discriminant = std::sqrt(discriminant);

        auto t = (h - discriminant) / a;

        if(t <= tMin || t >= tMax){
            // check other point
            t = (h + discriminant) / a;
            if(t <= tMin || t >= tMax)
                return std::nullopt;
        }

        hitInfo.t = t;
        hitInfo.hit = true;
        hitInfo.point = r.at(t);
        hitInfo.material = this->material;

        // this works because the vector is from the center
        // of the circle to a point on its surface, meaning
        // the length of it has to be equal to the radius
        //
        // dividing by the radius normalizes it
        vec3f outNormal = (hitInfo.point - center) / radius;

        hitInfo.setNormal(r, outNormal);

        return hitInfo;
    }
};

#endif
