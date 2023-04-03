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

	// whitted style ray tracing
	// color_t cast_ray(const Ray& ray, int depth);

	// path tracing
	vec3 cast_ray(const Ray& ray, int depth = 0);

	Intersection intersect(const Ray& ray);
	void sample_light(Intersection &inter, float &pdf_light);

	int width, height;
	double fovY;
	std::vector<Object*> objs;
	std::vector<Light> lights;
	BVH bvh;
	float russian_roulette = 0.8;
};