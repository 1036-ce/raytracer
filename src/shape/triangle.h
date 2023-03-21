#pragma once
#include <vector>
#include <utility>
#include <tuple>
#include "geometry.h"
#include "gl.h"
#include "box.h"
#include "object.h"

class Triangle : public Object
{
public:
	Triangle() = default;
	Triangle(Point3 pts[3], vec3 nrms[3], vec2 uv_coords[3], Material* m) {
		material = m;
		for (int i = 0; i < 3; ++i) {
			verts[i] = pts[i];
			normals[i] = nrms[i].normalize();
			uv[i] = uv_coords[i];
		}
	}

	Intersection intersect(const Ray& ray) override;
	bool is_intersect(const Ray& ray) override;
	Box  get_box()  override;
	float get_area() override;
	void sample(Intersection &inter, float &pdf) override;

	~Triangle() = default;
private:
	Point3 verts[3];
	vec3 normals[3];
	vec2 uv[3];
	Material *material;
};