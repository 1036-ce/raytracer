#pragma once

#include <vector>
#include "box.h"
#include "object.h"

class BVHNode;

class BVH {
public:
	BVH() = default;
	BVHNode* build(std::vector<Object*> objs);
	Intersection intersect(const Ray& ray);
	void sample(Intersection &inter, float &pdf);
	BVHNode *root = nullptr;
private:
	Intersection get_intersection(BVHNode *root, const Ray& ray);
	void get_sample(BVHNode *root, float p, Intersection &inter, float &pdf);
};

class BVHNode {
public:
	Box box;
	BVHNode *left;
	BVHNode *right;
	Object *object;
	float area;

	BVHNode() : box(Box()), left(nullptr), right(nullptr), object(nullptr) {}
};