#pragma once

#include "box.h"
#include "transform.h"
#include "intersection.h"

class Object {
public:
	Object() = default;
	Object(const Transform* w2o, const Transform* o2w)
		: world2object(w2o), object2world(o2w) {}
	~Object() = default;
	virtual Intersection intersect(const Ray& ray) = 0;
	virtual bool  is_intersect(const Ray& ray) = 0;
	virtual Box   get_box() = 0;
	virtual float get_area() = 0;
	virtual bool  is_emmision() { return false; };
	virtual void  sample(Intersection &inter, float &pdf) = 0;

	const Transform* world2object;
	const Transform* object2world;
};