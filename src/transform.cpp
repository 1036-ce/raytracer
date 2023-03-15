#include "transform.h"

vec3 Transform::operator()(const vec3 &v) const {
	vec3 ret(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
	);
	// Float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z;
	return ret;
}

Point3 Transform::operator()(const Point3 &p) const {
	Point3 ret(
		m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3],
		m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3],
		m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3]
	);
	Float w = m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3];
	return ret / w;
}

Ray Transform::operator()(const Ray &ray) const {
	Ray ret;
	ret.dir = (*this)(ray.dir);
	ret.orig = (*this)(ray.orig);
	return ret;
}

