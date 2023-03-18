#include "box.h"

Box Union(const Box &b1, const Box &b2) {
	Point3 ret_min, ret_max;
	for (int i = 0; i < 3; ++i) {
		ret_min[i] = std::min(b1.p_min[i], b2.p_min[i]);
		ret_max[i] = std::max(b1.p_max[i], b2.p_max[i]);
	}
	return Box(ret_min, ret_max);
}

Box Union(const Box &b, const Point3 &p) {
	Point3 ret_min, ret_max;
	for (int i = 0; i < 3; ++i) {
		ret_min[i] = std::min(b.p_min[i], p[i]);
		ret_max[i] = std::max(b.p_max[i], p[i]);
	}
	return Box(ret_min, ret_max);
}

bool Box::is_intersect(const Ray &ray) {
	double x_min = (p_min.x - ray.orig.x) / ray.dir.x;
	double x_max = (p_max.x - ray.orig.x) / ray.dir.x;
	if (ray.dir.x < 0)
		std::swap(x_min, x_max);

	double y_min = (p_min.y - ray.orig.y) / ray.dir.y;
	double y_max = (p_max.y - ray.orig.y) / ray.dir.y;
	if (ray.dir.y < 0)
		std::swap(y_min, y_max);

	double z_min = (p_min.z - ray.orig.z) / ray.dir.z;
	double z_max = (p_max.z - ray.orig.z) / ray.dir.z;
	if (ray.dir.z < 0)
		std::swap(z_min, z_max);

	double enter = std::max(x_min, std::max(y_min, z_min));
	double exit  = std::min(x_max, std::max(y_max, z_max));

	return enter <= exit && exit >= 0;
}