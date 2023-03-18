#pragma once

#include "global.h"
#include "geometry.h"

class Light {
public:
	Light() = default;
	Light(const vec3& p, const vec3& intens)
		: pos(p), intensity(intens) {}
	Light(const vec3& p, const color_t& c, const vec3& intens)
		: pos(p), color(c), intensity(intens) {}
	vec3 pos;
	color_t color;
	vec3 intensity;
};