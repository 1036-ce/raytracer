#pragma once

#include "global.h"
#include "color.h"

enum class MaterialType {
	DIFFUSE, REFLECT_REFRACT, REFLECT
};

class Material {
public:
	Material() = default;
	Material(MaterialType t, const vec3& e) : m_type(t), emit(e) {}
	bool is_emission() {
		return emit.norm() > 0.00001;
	}

	vec3 sample(const vec3& wi, const vec3& normal);

	float pdf(const vec3& wi, const vec3& wo, const vec3& normal);

	vec3 eval(const vec3& wi, const vec3& wo, const vec3& normal);

	MaterialType m_type;

	color_t Ka; 	// Ambient color;
	vec3 Kd;		// diffuse color
	color_t Ks;		// specular color;

	float Ns;		// specular exponent
    float Ni;		// Optical Density AKA 'index of refraction'

	vec3 emit;	
private:
	vec3 to_world(const vec3& v, const vec3& normal);
};