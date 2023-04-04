#include <cmath>
#include "microfacet.h"
#include "../core/gl.h"

vec3 MicorfacetMaterial::sample(const vec3 &wi, const vec3 &normal) {
	float a = random_float(), b = random_float();
	float alpha = this->roughness * this->roughness;
	float phi = 2 * b * PI;

	float ct = std::sqrt((1 - a)/(a * (alpha * alpha - 1) + 1));
	float st = std::sqrt(1 - ct * ct);
	float sp = std::sin(phi), cp = std::cos(phi);

	vec3 local_ray(st * sp, st * cp, ct);
	vec3 h = to_world(local_ray, normal).normalize();

	vec3 ret = reflect(h, wi);
	if (dot(ret, normal) < 0)
		return reflect(normal, ret);
	return ret;
}

float MicorfacetMaterial::pdf(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	vec3 v = wo, l = -wi, h = (v + l).normalize(), n = normal.normalize();
	float D = distributionGGX(n, h, this->roughness);
	float ct = std::clamp(dot(n, h), 0.f, 1.f);
	float VoH = std::max(dot(v, h), 0.001f);
	float ret = D * ct / (4 * VoH);
	ret = std::max(ret, 0.001f);
	return ret;
}

vec3 MicorfacetMaterial::eval(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	if (dot(wo, normal) <= 0)
		return vec3(0);

	vec3 v = wo, l = -wi, h = (v + l).normalize(), n = normal;
	float D = distributionGGX(n, h, this->roughness);
	vec3 f0 = mix(vec3(0.04), (vec3)this->albedo, this->metalic);
	vec3 F = fresnelSchlick(v, h, f0);
	float vis = Vis(n, v, l, this->roughness);

	vec3 spec(D * vis * F);
	vec3 diff = (vec3)this->albedo / PI;
	return spec + (1 - this->metalic) * diff;
}

float MicorfacetMaterial::distributionGGX(const vec3 &n, const vec3 &h, float roughness) {
	float a = roughness * roughness, a2 = a * a;
	float nh = std::max(dot(n, h), 0.001f);
	float div = nh * nh * (a2 - 1) + 1;
	div = PI * div * div;
	return a2 / div;
}

float MicorfacetMaterial::Vis(const vec3& n, const vec3& v, const vec3& l, float roughness) {
	float a = roughness * roughness + 1;
	float k = a * a * 0.125;

	auto f = [&](float t) -> float {
		float div = t * (1.f - k) + k;
		return 1.f / div;
	};

	float nv = std::max(dot(n, v), 0.f);
	float nl = std::max(dot(n, l), 0.f);

	float ret = f(nv) * f(nl) * 0.25;
	return ret;
}

vec3 MicorfacetMaterial::fresnelSchlick(const vec3 &v, const vec3 &n, const vec3& f0) {
	float c = 1 - std::max(dot(v, n), 0.001f);
	float x2 = c * c;
	float x5 = x2 * x2 * c;
	return f0 + (vec3(1) - f0) * x5;
}

vec3 MicorfacetMaterial::to_world(const vec3 &v, const vec3 &normal) {
	vec3 B, C;
	if (std::abs(normal.x) > std::abs(normal.y)) {
		float inv_len = 1.f / std::sqrt(normal.x * normal.x + normal.z * normal.z);
		C = vec3(normal.z * inv_len, 0, -normal.x * inv_len);
	}
	else {
		float inv_len = 1.f / std::sqrt(normal.y * normal.y + normal.z * normal.z);
		C = vec3(0, normal.z * inv_len, -normal.y * inv_len);
	}
	B = cross(C, normal);
	return v.x * B + v.y * C + v.z * normal;
}
