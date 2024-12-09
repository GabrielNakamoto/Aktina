#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "raytraceable.h"

class Material
{
public:

    virtual ~Material() = default;

    [[nodiscard]] virtual bool scatter(
        const Ray &incoming, const RayTraceable::Hit &hitInfo, vec3f &attentuation, Ray &scattered
    ) const = 0;
};


// what is albedo:
//
//      how much light on average an object reflects
//      low albedo -> dark
//      high albedo -> bright
//
// what is attentuation
//
// how do diffuse materials work?
//
// Materials can:
//
//      1. Scatter a (light) ray (while absorbing some of its energy)
//      2. Absorb a (light) ray
//
// If #1, attenuation describes the
// reduction of the intensity (or
// energy) of light
//
// After hitting an object some of the light
// gets absorb, decreasing its propogated
// intensity
//
// Albedo: Latin for 'whiteness'

class Lambertian : public Material
{
private:

    vec3f albedo;

public:

    Lambertian(const vec3f &albedo) : albedo(albedo)
    {
    };

    [[nodiscard]] bool scatter(const Ray &incoming, const RayTraceable::Hit &hitInfo, vec3f &attentuation, Ray &scattered) const override
    {
        // cosine weighted ray distribution
        // light rays closer to parallel to the normal
        // contribute more, therefore we should send
        // more rays in that direction
        auto scatterDirection = hitInfo.normal + randomUnitVector();

        scattered = Ray(hitInfo.point, scatterDirection);
        attentuation = albedo;

        return true;
    }

};

class Metal : public Material
{
private:

    vec3f albedo;

public:

    Metal(const vec3f &albedo) : albedo(albedo)
    {
    };

    [[nodiscard]] bool scatter(const Ray &incoming, const RayTraceable::Hit &hitInfo, vec3f &attentuation, Ray &scattered) const override
    {
        auto reflected = reflect(incoming.direction, hitInfo.normal);


        scattered = Ray(hitInfo.point, reflected);
        attentuation = albedo;

        return true;
    }

};

#endif
