#pragma once

#include "geometry.h"
#include "material.h"

class Object;

struct Intersection
{
	Intersection() {
		happened = false;
		material = nullptr;
	}
	bool happened;
	Float distance;
	Point3 pos;
	vec3 normal;
	vec2 uv;
	vec3 emit;
	Material *material;
	Object   *object;
};
