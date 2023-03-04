#pragma once

#include "global.h"
#include "color.h"

enum class MaterialType {
	DIFFUSE_GLOSSY, REFLECT_REFRACT, REFLECT
};

struct Material {
	MaterialType m_type;

	color_t Ka; 	// Ambient color;
	color_t Kd;		// diffuse color
	color_t Ks;		// specular color;

	float Ns;		// specular exponent
    float Ni;		// Optical Density AKA 'index of refraction'
};