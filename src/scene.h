#pragma once

#include <vector>
#include "global.h"
#include "color.h"
#include "object.h"
#include "light.h"
#include "BVH.h"

class Scene {
public:
	Scene() = default;
	Scene(int w, int h, double fovY_) 
		: width(w), height(h), fovY(fovY_) {}
	void add(Object* obj) { objs.push_back(obj); }
	void add(Light light) { lights.push_back(light); }
	void render();


private:
	void buildBVH();
	color_t cast_ray(const Ray& ray, int depth);
	Intersection intersect(const Ray& ray);

	int width, height;
	double fovY;
	std::vector<Object*> objs;
	std::vector<Light> lights;
	BVH bvh;
};