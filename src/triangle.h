#pragma once
#include <vector>
#include <utility>
#include <tuple>
#include "tgaimage.h"
#include "geometry.h"
#include "gl.h"
#include "ray.h"

using std::vector;
using std::tuple;

using pack_t = vector<tuple<int, depth_t, std::optional<color_t>>>;

class Triangle
{
public:
	Triangle() = default;
	Triangle(vec3 pts[3]) { for (int i = 3; i--; verts[i] = pts[i]); }
	Triangle(const vec3& A, const vec3& B, const vec3& C) {
		verts[0] = A;
		verts[1] = B;
		verts[2] = C;
	}

	bool intersect(const Ray& ray, vec3 &bar, float &near);
	~Triangle() = default;
private:
	vec3 verts[3];
};