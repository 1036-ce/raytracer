#include "ray.h"

vec3 Ray::at(double t) const {
	assert(t >= 0);
	return orig + dir * t;
}

std::ostream &operator<<(std::ostream &os, const Ray &ray) {
	os << "[Ray : (origin: " << ray.orig << " direction: " << ray.dir << ") ]\n";
	return os;
}