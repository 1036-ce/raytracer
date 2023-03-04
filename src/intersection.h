#pragma once

#include "geometry.h"
#include "material.h"

struct Intersection
{
	Intersection() {
		happened = false;
		material = nullptr;
	}
	bool happened;
	double distance;
	vec3 normal;
	vec2 uv;
	Material *material;
};
