#include <vector>
#include <algorithm>
#include "BVH.h"

BVHNode *BVH::build(std::vector<Object*> objs) {
	if (objs.size() == 0)
		return nullptr;

	BVHNode *node = new BVHNode();

	if (objs.size() == 1) {
		node->box = objs[0]->get_box();
		node->object = objs[0];
		node->left = nullptr;
		node->right = nullptr;
		node->area = objs[0]->get_area();
	}
	else if (objs.size() == 2) {
		node->left = build(std::vector<Object*>{objs[0]});
		node->right = build(std::vector<Object*>{objs[1]});

		node->box = Union(node->left->box, node->right->box);
		node->area = node->left->area + node->right->area;
	}
	else {
		Box box;
		for (int i = 0; i < objs.size(); ++i) {
			box = Union(box, objs[i]->get_box().centroid());
		}
		int edge = box.longest_edge();
		if (edge == 0) {
			std::sort(objs.begin(), objs.end(), [](auto s1, auto s2){
				return s1->get_box().centroid().x < s2->get_box().centroid().x;
			});
		}
		else if (edge == 1) {
			std::sort(objs.begin(), objs.end(), [](auto s1, auto s2){
				return s1->get_box().centroid().y < s2->get_box().centroid().y;
			});
		}
		else if (edge == 2) {
			std::sort(objs.begin(), objs.end(), [](auto s1, auto s2){
				return s1->get_box().centroid().z < s2->get_box().centroid().z;
			});
		}

		auto left  = objs.begin();
		auto mid   = objs.begin() + (objs.size() / 2);
		auto right = objs.end();

		auto left_part = std::vector<Object*>(left, mid);
		auto right_part = std::vector<Object*>(mid, right);

		node->left = build(left_part);
		node->right = build(right_part);

		node->box = Union(node->left->box, node->right->box);
		node->area = node->left->area + node->right->area;
	}

	return node;
}

Intersection BVH::intersect(const Ray &ray) {
	Intersection inter;
	if (!root)
		return inter;
	inter = get_intersection(root, ray);
	return inter;
}


Intersection BVH::get_intersection(BVHNode *root, const Ray &ray) {
	if (!root->box.is_intersect(ray))
		return Intersection();
	if (root->left && root->right) {
		auto r1 = get_intersection(root->left, ray);
		auto r2 = get_intersection(root->right, ray);
		if (r1.happened && r2.happened) {
			return r1.distance < r2.distance ? r1 : r2;
		}
		else 
			return r1.happened ? r1 : r2;
	}
	else {	// root->left == root->right = nullptr;
		auto ret = root->object->intersect(ray);
		return ret;
	}
}


void BVH::sample(Intersection &inter, float &pdf) {
	float p = sqrt(random_float()) * root->area;
	get_sample(root, p, inter, pdf);
	pdf /= root->area;
}

void BVH::get_sample(BVHNode *root, float p, Intersection &inter, float &pdf) {
	if (root->left == nullptr || root->right == nullptr) {
		root->object->sample(inter, pdf);
		pdf *= root->area;
		return;
	}

	if (p < root->left->area)
		get_sample(root->left, p, inter, pdf);
	else
		get_sample(root->right, p - root->left->area, inter, pdf);
}
