#pragma once
#include <vector>
#include <string>
#include <optional>
#include "geometry.h"
#include "gl.h"
#include "triangle.h"
#include "box.h"
#include "BVH.h"

/**
 vertex : v {x} {y} {z} [w]	; w is optional and defaults to 1.0
 		example : v 0.123 0.234 0.345 1.0
 texture coordinates : vt {u} [v] [w] vary between 0 and 1. v, w defaults to 0
		example : vt 0.500 0.123 0.231
 vertex normals : vn {x} {y} {z}
 		example : vn  0.001 0.482 -0.876
 face element : f {v}/[vt]/[vn]
		example : f 6/4/1 3/5/3 7/6/5

*/
class Model : public Object {
public:
	Model() = default;
	Model(const std::string& path, Material *m = nullptr);
	Intersection intersect(const Ray& ray) override;
	bool is_intersect(const Ray& ray) override;
	Box  get_box() override { return bbox;}
	float get_area() override;
	bool  is_emmision() override;
	void set_material(Material* m) { material = m;}
	void sample(Intersection &inter, float &pdf) override;
private:
	Box bbox;
	float area;
	std::vector<Triangle> triangles;
	Material *material = nullptr;
	BVH bvh;
};