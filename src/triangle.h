#pragma once
#include <vector>
#include <utility>
#include <tuple>
#include "geometry.h"
#include "gl.h"
#include "ray.h"
#include "box.h"
#include "object.h"

class Triangle : public Object
{
public:
	Triangle() = default;
	Triangle(vec3 pts[3], vec3 nrms[3], vec2 uv_coords[3]) {
		// vec3 nrm = cross(pts[1] - pts[0], pts[2] - pts[0]).normalize();
		for (int i = 0; i < 3; ++i) {
			verts[i] = pts[i];
			normals[i] = nrms[i].normalize();
			// normals[i] = nrm;
			uv[i] = uv_coords[i];
		}
	}

	Intersection intersect(const Ray& ray) override;
	Box  get_box()  override;

	void transform(const mat4& m);
	~Triangle() = default;
private:
	vec3 verts[3];
	vec3 normals[3];
	vec2 uv[3];
};