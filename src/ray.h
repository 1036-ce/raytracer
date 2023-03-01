#pragma once

#include "geometry.h"

class Ray {
public:
	Ray() = default;
	Ray(const vec3& orig_, const vec3& dir_)
		: orig(orig_), dir(dir_.normalize()) {}
	vec3 at(double t) const;

	vec3 orig;
	vec3 dir;
};

std::ostream &operator<<(std::ostream& os, const Ray& ray);
