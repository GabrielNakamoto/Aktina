
#include "common.h"
#include "scene.h"
#include "camera.h"


int main()
{
    Scene world;

    world.add(make_shared<Sphere>(vec3f(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(vec3f(0,-100.5,-1), 100));

    Camera cam(400, 16.0 / 9.0);

    cam.render(world);

    return 0;
}
