#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "framebuffer.h"
#include "scene.h"
#include "material.h"

#include "thread_pool.h"

class CameraBase
{
protected:

    int imageWidth, imageHeight;

    int samplesPerPixel;

    int maxDepth;

    FrameBuffer m_buffer;

    float aspectRatio;
    float focalLength;
    float viewportWidth, viewportHeight;

    vec3f position;
    vec3f originPixel;
    vec3f pixelDeltaRight, pixelDeltaDown;

public:

    virtual ~CameraBase()
    {

    }

    CameraBase(int samplesPerPixel, int imageWidth, float aspectRatio)
        :   imageWidth{ imageWidth }
        ,   imageHeight{ static_cast<int>(imageWidth / aspectRatio) }
        ,   samplesPerPixel(samplesPerPixel)
        ,   m_buffer{ FrameBuffer(imageWidth, imageHeight) }
        ,   aspectRatio{ aspectRatio }
    {

    }

	// render all function?
    void render(const Scene &scene, const std::string &filename)
    {
        initialize();

        float sampleWeight = 1.0 / samplesPerPixel;

		ThreadPool scheduler{};

		// split up image vertically
		int rowsPerWorker = imageHeight / scheduler.workers() + (imageHeight % scheduler.workers() > 0);

		// distribute work into jobs
		int endY {};

		while (endY < imageHeight)
		{
			int startY = endY;
			endY = std::min(startY + rowsPerWorker, imageHeight);

			scheduler.appendJob([this, &scene, startY, endY, sampleWeight]{
				for (int pixelY = startY; pixelY < endY; ++pixelY)
				{
					for (int pixelX {}; pixelX < imageWidth; ++pixelX)
					{
						vec3f pixelColor(0.0);

						for (int sample = 0; sample < this->samplesPerPixel; ++sample)
						{
							pixelColor  += this->shade(getRay(pixelX, pixelY), maxDepth, scene);
						}

						this->m_buffer.setPixel(pixelColor * sampleWeight, pixelX, pixelY);
					}
				}
			});
		}


		std::cout << "Waiting for completion..\n";

		scheduler.waitForCompletion();

        m_buffer.writeToPPM(filename);

        // output how long it took, samples per pixel + info?
    }

private:

    virtual void initialize() = 0;
    virtual vec3f shade(const Ray &r, int detph, const Scene &scene) const = 0;

    [[nodiscard]] Ray getRay(int pixelX, int pixelY) const
    {
        vec3f sample = this->sample();

        vec3f direction = originPixel + ((pixelX + sample.x) * pixelDeltaRight) + ((pixelY + sample.y) * pixelDeltaDown);
        // make direction go from camera position out
        direction = direction - this->position;

        return Ray(this->position, direction);
    }

    [[nodiscard]] vec3f sample() const
    {
        // -0.5 -> 0.5
        return vec3f(randomFloat() - 0.5, randomFloat() - 0.5, 0);
    }
};


class Camera : public CameraBase
{
public:

    Camera(int samplesPerPixel, int imageWidth, float aspectRatio)
        :   CameraBase(samplesPerPixel, imageWidth, aspectRatio)
    {

    }

private:

    void initialize()
    {
        focalLength = 1.0;

        maxDepth = 10;

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

    [[nodiscard]] vec3f shade(const Ray &r, int depth, const Scene &scene) const
    {
        if (depth <= 0)
            return vec3f(0.0);

        // ignore rays that are very close to surface
        if (auto hitInfo = scene.trace(r, 0.001, infinity))
        {
            Ray scattered;
            vec3f attenuation;

            if (hitInfo->material->scatter(r, hitInfo.value(), attenuation, scattered))
                return attenuation * shade(scattered, depth - 1, scene);

            return vec3f(0);
        }

        float a = 0.5 * (r.direction.y + 1.0);

        // lerp
        return (1.0 - a) * vec3f(1.0) + a * vec3f(0.5, 0.7, 1.0);
    }
};

#endif
