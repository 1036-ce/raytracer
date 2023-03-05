#include "triangle.h"

Intersection Triangle::intersect(const Ray &ray) {
	auto S = ray.orig - verts[0];
	auto E1 = verts[1] - verts[0];
	auto E2 = verts[2] - verts[0];
	auto S1 = cross(ray.dir, E2);
	auto S2 = cross(S, E1);

	float S1E1 = dot(S1, E1);
	float t = dot(S2, E2) / S1E1;
	float alpha = dot(S1, S) / S1E1;
	float beta = dot(S2, ray.dir) / S1E1;

	// std::cout << alpha << "  " << beta << std::endl;

	Intersection inter;
	if (t > 0.001 && 1 - alpha - beta >= 0 && alpha >= 0 && beta >= 0) {
		inter.happened = true;
		inter.distance = t;
		inter.uv = (1 - alpha - beta) * uv[0] + alpha * uv[1] + beta * uv[2];
		inter.normal = (1 - alpha - beta) * normals[0] + alpha * normals[1] + beta * normals[2];
	}
	return inter;
}

Box Triangle::get_box() {
	return Union(Box(verts[0], verts[1]), verts[2]);
}

void Triangle::transform(const mat4 &m) {
	mat4 mit = m.invert_transpose();
	for (int i = 0; i < 3; ++i) {
		verts[i] = vec3(m * vec4(verts[i], 1.0));
		normals[i] = vec3(mit * vec4(normals[i], 1.0));
	}
}
