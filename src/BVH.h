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
	BVHNode *root = nullptr;
private:
	Intersection get_intersection(BVHNode *root, const Ray& ray);
};

class BVHNode {
public:
	Box box;
	BVHNode *left;
	BVHNode *right;
	Object *object;

	BVHNode() : box(Box()), left(nullptr), right(nullptr), object(nullptr) {}
};