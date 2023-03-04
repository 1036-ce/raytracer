#pragma once
#include "color.h"

// global type alias
using color_t = Color;
using depth_t = float;

constexpr float PI = 3.141592653;

inline void update_progress(float progress)
{
    int width = 70;

    std::cout << "[";
    int pos = width * progress;
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
};