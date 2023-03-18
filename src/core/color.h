#pragma once
#include <algorithm>
#include "geometry.h"

// BGRA format
class Color {
public:
	float r{0}, g{0}, b{0}, a{1};
	Color() = default;
	Color(float _b) : b(_b) {}
	Color(float _r, float _g, float _b, float _a = 1.0)
		: r(_r), g(_g), b(_b), a(_a) { }
	float& operator[](int idx);
	explicit operator vec3() const {
		return vec3(r, g, b);
	};
private:
};

Color operator*(const Color& c, float intensity);
Color operator*(float intensity, const Color& c);
Color operator*(const Color &c, const vec3& v);
Color operator*(const vec3& v, const Color &c);
Color operator+(const Color& lhs, const Color& rhs);