#pragma once

#include "geometry.h"

class Box {
public:
	friend Box Union(const Box& b1, const Box& b2);
	friend Box Union(const Box& b,  const vec3& p);
	Box() {
		p_min = vec3(std::numeric_limits<double>::lowest());
		p_max = vec3(std::numeric_limits<double>::max());
	}
	Box(const vec3& p1, const vec3& p2) {
		p_min = vec3(fmin(p1.x, p2.x),fmin(p1.y, p2.y), fmin(p1.z, p2.z));
		p_max = vec3(fmax(p1.x, p2.x),fmax(p1.y, p2.y), fmax(p1.z, p2.z));
	}
private:
	vec3 p_min, p_max;
};

Box Union(const Box& b1, const Box& b2);

Box Union(const Box& b,  const vec3& p);