#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "framebuffer.h"


const auto focal_length = 1.0;

const int image_width = 400;
const auto aspect_ratio = 16.0 / 9.0;
const int image_height = image_width / aspect_ratio;
const auto viewport_height = 2.0;
const auto viewport_width = viewport_height * static_cast<float>(image_width) / image_height;

const auto viewport_u = vec3f(viewport_width, 0, 0);
const auto viewport_v = vec3f(0, -viewport_height, 0);

const auto pixel_delta_u = viewport_u / image_width;
const auto pixel_delta_v = viewport_v / image_height;

const auto camera_center = vec3f(0.0);

const auto viewport_upper_left = camera_center - vec3f(0,0,focal_length) - viewport_u / 2 - viewport_v / 2;
const auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

vec3f lerp(float a, const vec3f &start, const vec3f &end)
{
    return (static_cast<float>(1.0) - a) * start + a * end;
}


bool sphereIntersect(const vec3f &center, float radius, const Ray &r)
{
    vec3f oc = center - r.origin;
    auto b =  dot(oc, r.direction);
    auto c = dot(oc, oc) - radius*radius;

    auto discriminant = b*b - c;

    return (discriminant >= 0);
}

vec3f rayColor(Ray &r)
{
    if(sphereIntersect(vec3f(0,0,-1), 0.5, r))
        return vec3f(1.0, 0.0, 0.0);

    float a = 0.5 * (r.direction.y + 1.0);
    return lerp(a, vec3f(1.0), vec3f(0.5, 0.7, 1.0));
}

int main()
{

    Framebuffer buf(image_width, image_height);

    for(int i=0; i<image_height; ++i)
    {
        for(int j=0; j<image_width; ++j)
        {
            Ray cam;
            cam.origin = vec3f(0.0);

            vec3f pixelPos = pixel00_loc + (pixel_delta_v * i) + (pixel_delta_u * j);

            cam.direction = normalized(pixelPos - cam.origin);
            buf.setPixel(rayColor(cam), j, i);
        }
    }

    buf.writeToPPM("out.ppm");

    return 0;
}
