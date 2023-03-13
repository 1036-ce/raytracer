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
		inter.pos = ray.at(t);
		inter.uv = (1 - alpha - beta) * uv[0] + alpha * uv[1] + beta * uv[2];
		inter.normal = (1 - alpha - beta) * normals[0] + alpha * normals[1] + beta * normals[2];
	}
	return inter;
}

bool Triangle::is_intersect(const Ray &ray) {
	return false; 
}

Box Triangle::get_box() {
	return Union(Box(verts[0], verts[1]), verts[2]);
}

float Triangle::get_area() {
	vec3 e1 = verts[1] - verts[0];
	vec3 e2 = verts[2] - verts[0];
	return cross(e1, e2).norm() * 0.5f;
}

void Triangle::sample(Intersection &inter, float &pdf) {
	float r1 = sqrt(random_float()), r2 = random_float();
	float b0 = 1 - r1;
	float b1 = r1 * (1 - r2);
	float b2 = r1 * r2;
	inter.pos = b0 * verts[0] + b1 * verts[1] + b2 * verts[2];
	inter.normal = b0 * normals[0] + b1 * normals[1] + b2 * normals[2];
	pdf = 1.f / get_area();
}
