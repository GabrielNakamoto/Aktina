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

    bool trace(const Ray &r, Hit &hitInfo, float tMin, float tMax) const
    {
        float closestHit = tMax;
        bool intersection = false;

        for (const auto &object : objects)
        {
            if (object->intersect(r, hitInfo, tMin, closestHit))
            {
                intersection = true;
                closestHit = hitInfo.t;
            }
        }

        return intersection;
    }
};

#endif
