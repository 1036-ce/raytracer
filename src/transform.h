#pragma once

#include "geometry.h"

class Transform {
public:
	Transform() = default;
	Transform(const mat4& m_) : m(m_), m_inv(m_.invert()) {}
	Transform(const mat4& m_, const mat4 m_inv_)
		: m(m_), m_inv(m_inv_) {}
	
	vec3 operator()(const vec3& v) const;
	Point3 operator()(const Point3& p) const;
	Ray operator()(const Ray& ray) const;

	Transform inverse() const;
	Transform transpose() const;

	Transform operator*(const Transform& t) const {
		return Transform(this->m * t.m, t.m_inv * this->m_inv);
	}

private:
	mat4 m;
	mat4 m_inv;
};

Transform scale(Float x, Float y, Float z);
Transform translate(const vec3& delta);
Transform rotateX(Float angle);
Transform rotateY(Float angle);
Transform rotateZ(Float angle);
Transform rotate(Float angle, const vec3& axis);
Transform lookAt(const Point3& eye, const Point3& center, const vec3& up);