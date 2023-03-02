#include "box.h"

Box Union(const Box &b1, const Box &b2) {
	vec3 ret_min, ret_max;
	for (int i = 0; i < 3; ++i) {
		ret_min[i] = std::min(b1.p_min[i], b2.p_min[i]);
		ret_max[i] = std::max(b1.p_max[i], b2.p_max[i]);
	}
	return Box(ret_min, ret_max);
}

Box Union(const Box &b, const vec3 &p) {
	vec3 ret_min, ret_max;
	for (int i = 0; i < 3; ++i) {
		ret_min[i] = std::min(b.p_min[i], p[i]);
		ret_max[i] = std::max(b.p_max[i], p[i]);
	}
	return Box(ret_min, ret_max);
}
