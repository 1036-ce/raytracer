#pragma once

#include "../core/material.h"

class MirrorMaterial : public Material {
public:
	MirrorMaterial() = default;

	MaterialType type() override {
		return MaterialType::REFLECT;
	}

	vec3 sample(const vec3& wi, const vec3& normal) override;

	float pdf(const vec3& wi, const vec3& wo, const vec3& normal) override;

	vec3 eval(const vec3& wi, const vec3& wo, const vec3& normal) override;
private:
};