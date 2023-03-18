#pragma once
#include <iostream>
#include <random>

class Color;

// global type alias
using color_t = Color;
using depth_t = float;
using Float = float;

constexpr Float Infinity = std::numeric_limits<Float>::infinity();
constexpr float PI = 3.141592653;
constexpr Float MachineEpsilon = std::numeric_limits<Float>::epsilon() * 0.5;

constexpr Float gamma(int n) {
    return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
}

constexpr float radius(float angle) {
	return angle * 0.01745329251994329576923690768489;
}

constexpr float degree(float rad) {
	return rad / 0.01745329251994329576923690768489;
}


inline float random_float() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.f, 1.f);

    return dis(gen);
}

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