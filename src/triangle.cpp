#include "triangle.h"

bool Triangle::intersect(const Ray &ray, vec3 &bar, float &near) {
	auto S = ray.orig - verts[0];
	auto E1 = verts[1] - verts[0];
	auto E2 = verts[2] - verts[0];
	auto S1 = cross(ray.dir, E2);
	auto S2 = cross(S, E1);

	float S1E1 = dot(S1, E1);
	float t = dot(S2, E2) / S1E1;
	float alpha = dot(S1, S) / S1E1;
	float beta = dot(S2, ray.dir) / S1E1;

	if (t > 0.00001 && 1 - alpha - beta >= 0 && alpha >= 0 && beta >= 0) {
		bar = {1 - alpha - beta, alpha, beta};
		near = t;
		return true;
	}
	return false;
}