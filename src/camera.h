#pragma once
#include "geometry.h"
#include "ray.h"

class Camera {
public:
	Camera() = default;
	Camera(vec3 pos, vec3 target, vec3 up, int width, int height, float fovY);
	Ray get_ray(float x, float y);
	
private:
	vec3 pos;
	vec3 left_down;	// screen left down corner
	vec3 right;
	vec3 up;
	vec3 front;
};