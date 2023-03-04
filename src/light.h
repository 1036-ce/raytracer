#pragma once

#include "geometry.h"

class Light {
public:
	Light() = default;
	Light(const vec3& p, const vec3& intens)
		: pos(p), intensity(intens) {}
	vec3 pos;
	vec3 intensity;
};