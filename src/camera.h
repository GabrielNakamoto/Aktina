#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "framebuffer.h"
#include "scene.h"

class CameraBase
{
protected:

    int imageWidth, imageHeight;

    FrameBuffer m_buffer;

    float aspectRatio;
    float focalLength;
    float viewportWidth, viewportHeight;

    vec3f position;
    vec3f originPixel;
    vec3f pixelDeltaRight, pixelDeltaDown;

public:

    CameraBase(int imageWidth, float aspectRatio) :     imageWidth(imageWidth),
                                                    imageHeight(imageWidth / aspectRatio),
                                                    m_buffer(FrameBuffer(imageWidth, imageHeight)),
                                                    aspectRatio(aspectRatio)
    {

    }

    void render(const Scene &scene)
    {
        initialize();

        for(int pixelY = 0; pixelY < imageHeight; ++pixelY)
        {
            for (int pixelX = 0; pixelX < imageWidth; ++pixelX)
            {
                vec3f pixelColor = shade(getRay(pixelX, pixelY), scene);

                m_buffer.setPixel(pixelColor, pixelX, pixelY);
            }
        }

        m_buffer.writeToPPM("./out.ppm");
    }

private:

    virtual void initialize() = 0;
    virtual vec3f shade(const Ray &r, const Scene &scene) const = 0;

    Ray getRay(int pixelX, int pixelY)
    {
        vec3f direction = originPixel + (pixelX * pixelDeltaRight) + (pixelY * pixelDeltaDown);
        // make direction go from camera position out
        direction = direction - this->position;

        return Ray(this->position, direction);
    }
};


class Camera : public CameraBase
{
public:

    Camera(int imageWidth, float aspectRatio) : CameraBase(imageWidth, aspectRatio)
    {

    }

private:

    void initialize()
    {
        focalLength = 1.0;

        viewportHeight = 2.0;
        viewportWidth = viewportHeight * static_cast<float>(imageWidth) / imageHeight;

        const auto viewportRight = vec3f(viewportWidth, 0, 0);
        const auto viewportDown = vec3f(0, -viewportHeight, 0);

        pixelDeltaRight = viewportRight / imageWidth;
        pixelDeltaDown = viewportDown / imageHeight;

        // find top left
        originPixel = position - vec3f(0, 0, focalLength) - (viewportRight / 2) - (viewportDown / 2);
        // move to the center of the pixel
        originPixel += 0.5 * (pixelDeltaRight + pixelDeltaDown);
    }

    vec3f shade(const Ray &r, const Scene &scene) const
    {
        Hit hitInfo;

        if (scene.trace(r, hitInfo, 0, infinity))
        {
            return 0.5 * (hitInfo.normal + vec3f(1.0));
        }

        float a = 0.5 * (r.direction.y + 1.0);

        // lerp
        return [&]
        {
            return (1.0 - a) * vec3f(1.0) + a * vec3f(0.5, 0.7, 1.0);
        }();
    }
};

#endif
