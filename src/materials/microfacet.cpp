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

	vec3 local_ray(st * sp, st * cp, ct);	// maybe
	vec3 h = to_world(local_ray, normal).normalize();

	vec3 ret = reflect(h, wi);
	if (dot(ret, normal) <= 0) {
		return vec3(0);
	}

	return ret;
}

float MicorfacetMaterial::pdf(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	vec3 wh = (wo - wi).normalize();
	float c = dot(wh, normal);
	float s = std::sqrt(1 - c * c);
	float alpha2 = this->roughness * this->roughness;
	alpha2 = alpha2 * alpha2;
	float down = c * c * (alpha2 - 1) + 1;
	down = PI * down * down;
	float up = alpha2 * c;
	float ret = up / down;
	return ret / (4 * dot(wo, wh));
}

vec3 MicorfacetMaterial::eval(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	if (dot(wo, normal) <= 0)
		return vec3(0);
	// vec3 v = -wi, l = wo, h = (v + l).normalize(), n = normal;
	vec3 v = wo, l = -wi, h = (v + l).normalize(), n = normal;
	float D = distributionGGX(n, h, this->roughness);
	float G = geometrySmith(n, v, l, this->roughness);
	float F = fresnelSchlick(v, n, this->eta);

	vec3 a(D * G * F);
	// float b = 4 * std::max(dot(n, l), 0.00001f) * std::max(dot(n, v), 0.00001f);
	float b = 4 * dot(n, l) * dot(n, v);
	vec3 spec = a / b; 
	// vec3 spec = (vec3)this->Ks * a / b; 
	vec3 diff((vec3)this->Kd / PI);

	return spec + diff;
	// return diff;
}

float MicorfacetMaterial::distributionGGX(const vec3 &n, const vec3 &h, float roughness) {
	float a = roughness * roughness, a2 = a * a;
	float nh = dot(n, h);
	float div = nh * nh * (a2 - 1) + 1;
	div = PI * div * div;
	// return a2 / std::max(div, 0.00001f);
	return a2 / div;
}

float MicorfacetMaterial::geometrySchlickGGX(float nv, float k) {
	float div = nv / (1.0 - k) + k;
	return nv / div;
}

float MicorfacetMaterial::geometrySmith(const vec3 &n, const vec3 &v, const vec3 &l, float roughtness) {
	float r = roughtness + 1.f;
	float k = r * r * 0.125f;
	float nv = std::max(dot(n, v), 0.f);
	float nl = std::max(dot(n, l), 0.f);
	float ggx1 = geometrySchlickGGX(nv, k);
	float ggx2 = geometrySchlickGGX(nl, k);
	return ggx1 * ggx2;
}

float MicorfacetMaterial::fresnelSchlick(const vec3 &v, const vec3 &n, float eta) {
	float f0 = (eta - 1.f) / (eta + 1.f);
	f0 = f0 * f0;
	float c = 1 - dot(v, n);
	float x2 = c * c;
	float x5 = x2 * x2 * c;
	return f0 + (1 - f0) * x5;
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
