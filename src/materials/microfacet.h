#pragma once

#include "../core/material.h"

class MicorfacetMaterial : public Material {
public:
	MicorfacetMaterial() = default;
	MicorfacetMaterial(const color_t& albedo, float rough, float metalic)
		: albedo(albedo), roughness(rough), metalic(metalic) {}

	MaterialType type() override {
		return MaterialType::MICROFACET;
	}

	vec3 sample(const vec3& wi, const vec3& normal) override;

	float pdf(const vec3& wi, const vec3& wo, const vec3& normal) override;

	// https://zhuanlan.zhihu.com/p/152226698
	vec3 eval(const vec3& wi, const vec3& wo, const vec3& normal) override;
private:
  	// https://www.yuque.com/gaoshanliushui-mbfny/sst4c5/az43kq#lf572
  	// normal distribution funciton
  	float distributionGGX(const vec3 &n, const vec3 &h, float roughness);

	float Vis(const vec3 &n, const vec3 &v, const vec3 &l, float roughness);

	// https://zhuanlan.zhihu.com/p/303168568
	vec3 fresnelSchlick(const vec3& h, const vec3& v, const vec3& k);

	vec3 to_world(const vec3 &v, const vec3 &normal);

	float roughness;
	color_t albedo;
	float metalic;
	float ppdf;
	float nndf;
};