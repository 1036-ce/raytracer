#include "camera.h"

Camera::Camera(Point3 pos_, Point3 target, vec3 up_, int width, int height, float fovY) {
	pos = pos_;
	front = (target - pos_).normalize();
	right = cross(front, up_).normalize();
	up = cross(right, front).normalize();

	float depth = height / (2.f * tanf(fovY / 2.f));
	left_down = pos + front * depth - right * (width / 2.f) - up * (height / 2.f);
}

Ray Camera::get_ray(float x, float y) {
	Point3 pixel = left_down + right * x + up * y;
	vec3 dir = pixel - Point3(0, 0, 0);
	return Ray(pos, dir);
}
