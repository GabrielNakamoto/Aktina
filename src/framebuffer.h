#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <iostream>
#include <vector>
#include <fstream>

#include "vec3.h"


class Framebuffer
{
private:

    int width, height;
    std::unique_ptr<std::vector<vec3i>> m_buffer;

    void printPixel(std::ostream& out, int pixel_x, int pixel_y){
        vec3<int> pixel = (*m_buffer)[pixel_y * width + pixel_x];
        out << pixel.x << ' ' << pixel.y << ' ' << pixel.z << '\n';
    }

public:

    Framebuffer(int width, int height) : width(width), height(height), m_buffer(std::make_unique<std::vector<vec3i>>(std::vector<vec3i>(width * height)))
    {

    };

    void setPixel(const vec3f &color, int pixel_x, int pixel_y)
    {
        auto red = static_cast<int>(255.999 * color.x);
        auto green = static_cast<int>(255.999 * color.y);
        auto blue = static_cast<int>(255.999 * color.z);

        (*m_buffer)[pixel_y * width + pixel_x] = vec3i(red, green, blue);
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
