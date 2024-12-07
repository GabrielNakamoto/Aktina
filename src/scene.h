#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "common.h"
#include "raytraceable.h"

class Scene
{
public:

    std::vector<shared_ptr<RayTraceable>> objects;

public:

    Scene()
    {

    }

    void clear()
    {
        objects.clear();
    }

    void add(shared_ptr<RayTraceable> object)
    {
        objects.push_back(object);
    }

    [[nodiscard]] std::optional<RayTraceable::Hit> trace(const Ray &r, float tMin, float tMax) const
    {
        RayTraceable::Hit hitInfo;

        float closestHit = tMax;
        bool intersection = false;

        for (const auto &object : objects)
        {
            if (auto hit = object->intersect(r, tMin, closestHit))
            {
                intersection = true;
                hitInfo = hit.value();
                closestHit = hitInfo.t;
            }
        }

        if(! intersection) return std::nullopt;

        return hitInfo;
    }
};

#endif
