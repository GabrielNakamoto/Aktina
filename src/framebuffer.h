#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <iostream>
#include <vector>
#include <fstream>

#include "vec3.h"


class FrameBuffer
{
private:

    int width, height;
    std::vector<vec3i> m_buffer;

    void printPixel(std::ostream& out, int pixel_x, int pixel_y){
        vec3<int> pixel = m_buffer[pixel_y * width + pixel_x];
        out << pixel.x << ' ' << pixel.y << ' ' << pixel.z << '\n';
    }

    vec3f gammaCorrect(const vec3f &color)
    {
        auto _correct = [](float c) -> float
        {
            return (c > 0) ? std::sqrt(c) : c;
        };

        return vec3f(_correct(color.x), _correct(color.y), _correct(color.z));
    }

    vec3f clampColor(const vec3f &color, float low, float high)
    {
        auto _clamp = [&](float c) -> float
        {
            if(c < low) return low;
            if(c > high) return high;
            return c;
        };

        return vec3f(_clamp(color.x), _clamp(color.y), _clamp(color.z));
    }

public:

    FrameBuffer(int width, int height) : width(width), height(height), m_buffer(std::vector<vec3i>(std::vector<vec3i>(width * height)))
    {

    };

    void setPixel(const vec3f &color, int pixel_x, int pixel_y)
    {
        vec3f correctedColor = clampColor(gammaCorrect(color), 0.0, 1.0);

        auto red = static_cast<int>(std::round(255.0 * correctedColor.x));
        auto green = static_cast<int>(std::round(255.0 * correctedColor.y));
        auto blue = static_cast<int>(std::round(255.0 * correctedColor.z));

        m_buffer[pixel_y * width + pixel_x] = vec3i(red, green, blue);
    }


    void writeToPPM(const std::string &filename)
    {
        std::ofstream file;
        file.open(filename);

        file << "P3\n" << width << ' ' << height << "\n255\n";

        for(int py = 0; py < height; ++py){
            for(int px = 0; px < width; ++px){
                printPixel(file, px, py);
            }
        }

        file.close();
    }
};

#endif
