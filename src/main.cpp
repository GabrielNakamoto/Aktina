
#include "common.h"
#include "scene.h"
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

vec3f rayColor(Ray &r, const Scene &scene)
{
    Hit hitInfo;

    if (scene.trace(r, hitInfo, 0, infinity))
    {
        return 0.5 * (hitInfo.normal + vec3f(1.0));
    }

    float a = 0.5 * (r.direction.y + 1.0);
    return lerp(a, vec3f(1.0), vec3f(0.5, 0.7, 1.0));
}

int main()
{
    Scene world;

    world.add(make_shared<Sphere>(vec3f(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(vec3f(0,-100.5,-1), 100));

    Framebuffer buf(image_width, image_height);

    for(int i=0; i<image_height; ++i)
    {
        for(int j=0; j<image_width; ++j)
        {
            Ray cam;
            cam.origin = vec3f(0.0);

            vec3f pixelPos = pixel00_loc + (pixel_delta_v * i) + (pixel_delta_u * j);

            cam.direction = pixelPos - cam.origin;
            buf.setPixel(rayColor(cam, world), j, i);
        }
    }

    buf.writeToPPM("./out.ppm");

    return 0;
}
