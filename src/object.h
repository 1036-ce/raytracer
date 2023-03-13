#pragma once

#include "ray.h"
#include "box.h"
#include "intersection.h"

class Object {
public:
	Object() = default;
	~Object() = default;
	virtual Intersection intersect(const Ray& ray) = 0;
	virtual bool  is_intersect(const Ray& ray) = 0;
	virtual Box   get_box() = 0;
	virtual float get_area() = 0;
	virtual bool  is_emmision() { return false; };
	virtual void  sample(Intersection &inter, float &pdf) = 0;
};