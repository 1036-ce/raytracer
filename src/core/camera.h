#pragma once
#include "geometry.h"

class Camera {
public:
	Camera() = default;
	Camera(Point3 pos, Point3 target, vec3 up, int width, int height, float fovY);
	Ray get_ray(float x, float y);
	
private:
	Point3 pos;
	Point3 left_down;	// screen left down corner
	vec3 right;
	vec3 up;
	vec3 front;
};