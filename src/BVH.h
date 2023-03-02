#pragma once

#include "box.h"
#include "triangle.h"

class BVH {
public:
	BVH() = default;
	BVHNode* build();
	BVHNode *root = nullptr;
private:
};

class BVHNode {
public:
	Box box;
	BVHNode *left;
	BVHNode *right;
	Triangle *tri;

	BVHNode() : box(Box()), left(nullptr), right(nullptr), tri(nullptr) {}
};