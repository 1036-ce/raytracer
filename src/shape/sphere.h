#pragma once

#include <utility>
#include "../core/object.h"
#include "../core/material.h"

class Sphere : public Object {
public:
	Sphere() : center(Point3()), radius(0), material(nullptr) {}
	Sphere(const Point3& c, float r, Material* m)
		: center(c), radius(r), material(m) {}

	Intersection intersect(const Ray& ray) override;
	bool  is_intersect(const Ray& ray) override;
	Box   get_box() override;
	float get_area() override;
	void  sample(Intersection &inter, float &pdf) override;
	bool is_emmision() override {
		return material->type() == MaterialType::EMITTER;
	}
private:
	Point3 center;
	float radius;
	Material *material;

	std::pair<float, float> quadratic(float A, float B, float C);
};