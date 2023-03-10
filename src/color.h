#pragma once
#include <algorithm>
#include "geometry.h"

// BGRA format
class Color {
public:
	float r{0}, g{0}, b{0}, a{1};
	Color() = default;
	Color(float _b) { b = std::clamp(_b, 0.f, 1.f); }
	Color(float _r, float _g, float _b, float _a = 1.0) {
		r = std::clamp(_r, 0.f, 1.f);
		g = std::clamp(_g, 0.f, 1.f);
		b = std::clamp(_b, 0.f, 1.f);
		a = std::clamp(_a, 0.f, 1.f);
	}
	float& operator[](int idx);
private:
};

Color operator*(const Color& c, float intensity);
Color operator*(float intensity, const Color& c);
Color operator*(const Color &c, const vec3& v);
Color operator*(const vec3& v, const Color &c);
Color operator+(const Color& lhs, const Color& rhs);