#pragma once

#include "ray.h"
#include "box.h"
#include "intersection.h"

class Object {
public:
	Object() = default;
	~Object() = default;
	virtual Intersection intersect(const Ray& ray) = 0;
	virtual Box  get_box() = 0;
};