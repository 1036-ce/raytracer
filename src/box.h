#pragma once

#include "ray.h"
#include "geometry.h"

class Box {
public:
	friend Box Union(const Box& b1, const Box& b2);
	friend Box Union(const Box& b,  const vec3& p);
	Box() {
		p_min = vec3(std::numeric_limits<double>::max());
		p_max = vec3(std::numeric_limits<double>::lowest());
	}
	Box(const vec3& p1, const vec3& p2) {
		p_min = vec3(fmin(p1.x, p2.x),fmin(p1.y, p2.y), fmin(p1.z, p2.z));
		p_max = vec3(fmax(p1.x, p2.x),fmax(p1.y, p2.y), fmax(p1.z, p2.z));
	}

	bool is_intersect(const Ray& ray);

	// the center point of the Box
	vec3 centroid() { return (p_min + p_max) * 0.5;}

	// 0 == x; 1 == y; 2 == z;
	int longest_edge() {
		double dx = p_max.x - p_min.x;
		double dy = p_max.y - p_min.y;
		double dz = p_max.z - p_min.z;

		if (dx > dy && dx > dz) 	 return 0;
		else if (dy > dx && dy > dz) return 1;
		else 						 return 2;
	}
private:
	vec3 p_min, p_max;
};

Box Union(const Box& b1, const Box& b2);

Box Union(const Box& b,  const vec3& p);