#include "sphere.h"

Intersection Sphere::intersect(const Ray &ray) {
	Intersection inter;

	float A = ray.dir.norm2();
	vec3 tmp = ray.orig - this->center;
	float B = 2 * dot(tmp, ray.dir);
	// float C = vec3(this->center).norm2() - this->radius * this->radius;
	float C = tmp.norm2() - this->radius * this->radius;
	auto [t_min, t_max] = quadratic(A, B, C);
	if (t_min == Infinity && t_max == Infinity)
		return inter;
	
	if (t_max < 0)
		return inter;

	float t = t_min < 0 ? t_max : t_min;
	inter.happened = true;
	inter.distance = t;
	inter.material = this->material;
	inter.object = this;
	inter.pos = ray.at(t);
	inter.normal = (inter.pos - this->center).normalize();
	inter.uv = vec2();

	return inter;
}

bool Sphere::is_intersect(const Ray &ray) {
	float A = ray.dir.norm2();
	vec3 tmp = ray.orig - this->center;
	float B = 2 * dot(tmp, ray.dir);
	float C = vec3(this->center).norm2() - this->radius * this->radius;
	auto [t_min, t_max] = quadratic(A, B, C);
	if (t_min == Infinity && t_max == Infinity)
		return false;
	
	if (t_max < 0)
		return false;

	return true;
}

Box Sphere::get_box() {
	Point3 p1 = center - vec3(radius, radius, radius);
	Point3 p2 = center + vec3(radius, radius, radius);
	return Box(p1, p2);
}

float Sphere::get_area() {
	return 4 * PI * radius * radius;
}

void Sphere::sample(Intersection &inter, float &pdf) {
	float theta = 2 * PI * random_float();
	float phi = PI * random_float();

	float st = std::sin(theta), sp = std::sin(phi);
	float ct = std::cos(theta), cp = std::cos(phi);

	vec3 dir(cp, sp * ct, sp * st);

	inter.pos = this->center + this->radius * dir;
	inter.normal = dir;
	inter.material = this->material;
	// inter.pos = Point3(radius * st * cp, radius * st * sp, radius * cp) ;
	// inter.normal = (inter.pos - center).normalize();
	// inter.material = this->material;

	pdf = 1.f / get_area();
}

std::pair<float, float> Sphere::quadratic(float A, float B, float C) {
	float delta = B * B - 4 * A * C;
	if (delta < 0)
		return {Infinity, Infinity};
	else if (delta == 0)
		return {-0.5 * B / A, -0.5 * B / A};

	delta = std::sqrt(delta);
	float t1 = -B - delta, t2 = -B + delta;
	A = 2 * A;
	t1 = t1 / A;
	t2 = t2 / A;
	if (t1 > t2) std::swap(t1, t2);
	return {t1, t2};
}
