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

Transform Transform::inverse() const {
	return Transform(m_inv, m);
}

Transform Transform::transpose() const {
	return Transform(m.transpose(), m_inv.transpose());
}

Transform scale(Float x, Float y, Float z) {
	mat4 ret = mat4::identity();
	ret[0][0] = x;
	ret[1][1] = y;
	ret[2][2] = z;

	mat4 ret_inv = mat4::identity();
	ret_inv[0][0] = (Float)1 / x;
	ret_inv[1][1] = (Float)1 / y;
	ret_inv[2][2] = (Float)1 / z;

	return Transform(ret, ret_inv);
}

Transform translate(const vec3 &delta) {
	mat4 ret = mat4::identity();
	ret[0][3] = delta.x;
	ret[1][3] = delta.y;
	ret[2][3] = delta.z;

	mat4 ret_inv = mat4::identity();
	ret_inv[0][3] = -(delta.x);
	ret_inv[1][3] = -(delta.y);
	ret_inv[2][3] = -(delta.z);

	return Transform(ret, ret_inv);
}

Transform rotateX(Float angle) {
	Float rad = radian(angle);
	Float c = std::cos(rad);
	Float s = std::sin(rad);

	mat4 ret = mat4::identity();
	ret[1][1] = c;
	ret[1][2] = -s;
	ret[2][1] = s;
	ret[2][2] = c;

	return Transform(ret, ret.transpose());
}

Transform rotateY(Float angle) {
	Float rad = radian(angle);
	Float c = std::cos(rad);
	Float s = std::sin(rad);

	mat4 ret = mat4::identity();
	ret[0][0] = c;
	ret[0][2] = s;
	ret[2][0] = -s;
	ret[2][2] = c;

	return Transform(ret, ret.transpose());
}

Transform rotateZ(Float angle) {
	Float rad = radian(angle);
	Float c = std::cos(rad);
	Float s = std::sin(rad);

	mat4 ret = mat4::identity();
	ret[0][0] = c;
	ret[0][1] = -s;
	ret[1][0] = s;
	ret[1][1] = c;
	
	return Transform(ret, ret.transpose());
}

Transform rotate(Float angle, const vec3 &axis) {
	Float rad = radian(angle);
	Float s = std::sin(rad);
	Float c = std::cos(rad);

	vec3 a = axis.normalize();
	vec3 tmp  = (1.0 - c) * a;

	mat4 ret = mat4::identity();

	ret[0][0] = tmp[0] * a[0] + c;
	ret[0][1] = tmp[0] * a[1] - s * a[2];
	ret[0][2] = tmp[0] * a[2] + s * a[1];

	ret[1][0] = tmp[1] * a[0] + s * a[2];
	ret[1][1] = tmp[1] * a[1] + c;
	ret[1][2] = tmp[1] * a[2] - s * a[0];

	ret[2][0] = tmp[2] * a[0] - s * a[1];
	ret[2][1] = tmp[2] * a[1] + s * a[0];
	ret[2][2] = tmp[2] * a[2] + c;

	return Transform(ret, ret.transpose());
}

Transform lookAt(const Point3 &eye, const Point3 &center, const vec3 &up) {
	vec3 z = (eye - center).normalize();
	vec3 x = cross(up, z).normalize();
	vec3 y = cross(z, x).normalize();

	mat4 camera_to_world;
	camera_to_world.set_col(0, vec4(x, 0));
	camera_to_world.set_col(1, vec4(y, 0));
	camera_to_world.set_col(2, vec4(z, 0));
	camera_to_world.set_col(3, vec4(eye.x, eye.y, eye.z, 1));

	return Transform(camera_to_world.invert(), camera_to_world);
}
