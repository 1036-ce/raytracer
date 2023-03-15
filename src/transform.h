#pragma once

#include "geometry.h"
#include "ray.h"

class Transform {
public:
	Transform() = default;
	Transform(const mat4& m_) : m(m_), m_inv(m_.invert()) {}
	Transform(const mat4& m_, const mat4 m_inv_)
		: m(m_), m_inv(m_inv_) {}
	
	vec3 operator()(const vec3& v) const;
	Point3 operator()(const Point3& p) const;
	Ray operator()(const Ray& ray) const;
private:
	mat4 m;
	mat4 m_inv;
};