#include "common.h"
#include "scene.h"
#include "camera.h"
#include "material.h"
#include "raytraceable.h"


int main(int argc, char **argv)
{
    int spp = (argc == 1) ? 100 : atoi(argv[1]);

    Scene world;

    auto material_ground = make_shared<Lambertian>(vec3f(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(vec3f(0.1, 0.2, 0.5));
    auto material_left   = make_shared<Metal>(vec3f(0.8, 0.8, 0.8), 0.4);
    auto material_right  = make_shared<Metal>(vec3f(0.8, 0.6, 0.2), 0.1);

    world.add(make_shared<Sphere>(vec3f( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(vec3f( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<Sphere>(vec3f(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(vec3f( 1.0,    0.0, -1.0),   0.5, material_right));

    Camera cam(spp, 400, 16.0 / 9.0);

    cam.render(world, "./out.ppm");

    return 0;
}
