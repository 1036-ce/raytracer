#include <cassert>
#include <cmath>
#include "color.h"

// BGRA
float& Color::operator[](int idx) {
	if (idx < 0 || idx > 4)
		std::abort();
	if (idx == 0) 	   return b;
	else if (idx == 1) return g;
	else if (idx == 2) return r;
	else 			   return a;
}

Color operator*(const Color &c, float intensity) {
	// assert(intensity >= 0.0 && intensity <= 1.0);
	Color ret;
	ret.r = c.r * intensity;
	ret.g = c.g * intensity;
	ret.b = c.b * intensity;
	ret.a = c.a * intensity;
	return ret;
}

Color operator*(float intensity, const Color &c) {	
	Color ret;
	ret.r = c.r * intensity;
	ret.g = c.g * intensity;
	ret.b = c.b * intensity;
	ret.a = c.a * intensity;
	return ret;
}

Color operator*(const Color &c, const vec3& v) {
	Color ret;
	ret.r = c.r * v.x;
	ret.g = c.g * v.y;
	ret.b = c.b * v.z;
	return ret;
}

Color operator*(const vec3& v, const Color &c) {
	Color ret;
	ret.r = c.r * v.x;
	ret.g = c.g * v.y;
	ret.b = c.b * v.z;
	return ret;
}

Color operator+(const Color &lhs, const Color &rhs) {
	Color ret;
	ret.r = lhs.r + rhs.r;
	ret.g = lhs.g + rhs.g;
	ret.b = lhs.b + rhs.b;
	ret.a = lhs.a + rhs.a;
	return ret;
}
