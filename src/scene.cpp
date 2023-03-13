#include "scene.h"
#include "gl.h"
#include "tgaimage.h"
#include "buffer.h"
#include "camera.h"

void Scene::render() {
	buildBVH();

	ColorBuffer color_buf(width, height, color_t(0, 0, 0));
    float scale = tan((fovY * 0.5));
    float aspect= width / (float)height;
	vec3 eye_pos(278, 273, -800);

	int spp = 16;
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
            float x = (2 * (i + 0.5) / (float)width - 1) * aspect* scale;
            float y = (2 * (j + 0.5) / (float)height - 1) * scale;

			Ray ray(eye_pos, vec3(-x, y, 1).normalize());
			vec3 tmp(0);
			for (int k = 0; k < spp; k++){
				tmp += (cast_ray(ray) / spp);
            }
			color_t color(tmp.x, tmp.y, tmp.z);
			// if (vec3(color.r, color.g, color.b).norm() > 0.001)
			if (tmp.norm() > 0.001)
				int a = 1;
			color_buf.set(i, j, 0, color);
		}
		update_progress((float)j / height);
	}
	update_progress(1.0);

	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			image.set(i, j, color_buf.get_value(i, j));
		}
	}
	image.write_tga_file("output.tga");
	system("convert output.tga output.png");
	system("mv output.png ../");
}

void Scene::buildBVH() {
	bvh.root = bvh.build(objs);
}

// whitted style ray tracing
// color_t Scene::cast_ray(const Ray &ray, int depth) {
// 	if (depth > 4)
// 		return color_t(0.2, 0.6, 0.8);
// 	auto inter = intersect(ray);
// 	color_t hit_color(0.2, 0.6, 0.8);
// 	if (inter.happened) {
// 		switch (inter.material->m_type)
// 		{
// 			case MaterialType::DIFFUSE: {
// 				vec3 pt = ray.at(inter.distance);
// 				vec3 normal = inter.normal.normalize();
// 				vec3 shadow_pt = dot(ray.dir, normal) > 0 ? pt - normal * 0.00001 : pt + normal * 0.00001;
// 				vec3 diff_intensity(0);
// 				vec3 spec_intensity(0);
// 				for (Light light: lights) {
// 					vec3 light_dir = light.pos - pt;
// 					float light_dist2 = light_dir.norm2();
// 					light_dir = light_dir.normalize();
// 					bool shadow = bvh.intersect(Ray(shadow_pt, light_dir)).happened;
// 					float factor = std::max(0.0, dot(light_dir, normal));
// 					diff_intensity += shadow ? vec3(0) : ((factor / light_dist2) * light.intensity);
// 					vec3 reflect_dir = reflect(normal, -light_dir);
// 					vec3 halfway_dir = ( -ray.dir + light_dir).normalize();
// 					factor = std::max(0.0, -dot(reflect_dir, ray.dir));
// 					spec_intensity += shadow ? vec3(0) : (pow(factor, inter.material->Ns) / light_dist2 * light.intensity);
// 				}
// 				hit_color =  inter.material->Kd * diff_intensity + inter.material->Ks * spec_intensity; 
// 				break;
// 			}
// 			case MaterialType::REFLECT_REFRACT : {
// 				vec3 pt = ray.at(inter.distance);
// 				vec3 normal = inter.normal.normalize();
// 				vec3 shadow_pt = dot(ray.dir, normal) > 0 ? pt - normal * 0.00001 : pt + normal * 0.00001;
// 				vec3 reflect_dir = reflect(normal, ray.dir);
// 				vec3 reflect_pt = dot(reflect_dir, normal) > 0 ? (pt + normal * 0.00001) : (pt - normal * 0.00001);
// 				color_t reflect_color = cast_ray(Ray(reflect_pt, reflect_dir), depth + 1);
// 				auto refract_dir = refract(normal, ray.dir, inter.material->Ni);
// 				color_t refract_color = {0, 0, 0};
// 				if (refract_dir.has_value()) {
// 					vec3 refract_pt = dot(refract_dir.value(), normal) > 0 ? (pt + normal * 0.00001) : (pt - normal * 0.00001);
// 					refract_color = cast_ray(Ray(pt, refract_dir.value()), depth + 1);
// 				}
// 				vec3 diff_intensity(0);
// 				vec3 spec_intensity(0);
// 				for (Light light: lights) {
// 					vec3 light_dir = light.pos - pt;
// 					float light_dist2 = light_dir.norm2();
// 					light_dir = light_dir.normalize();
// 					bool shadow = bvh.intersect(Ray(shadow_pt, light_dir)).happened;
// 					float factor = std::max(0.0, dot(light_dir, normal));
// 					diff_intensity += shadow ? vec3(0) : ((factor / light_dist2) * light.intensity);
// 					vec3 reflect_dir = reflect(normal, -light_dir);
// 					vec3 halfway_dir = ( -ray.dir + light_dir).normalize();
// 					factor = std::max(0.0, -dot(reflect_dir, ray.dir));
// 					spec_intensity += shadow ? vec3(0) : (pow(factor, inter.material->Ns) / light_dist2 * light.intensity);
// 				}
// 				float fres = fresnel(ray.dir, normal, inter.material->Ni);
// 				// if (refract_color.r == 0 && refract_color.g == 0 && refract_color.b == 0)
// 				// 	std::cout << 'a' << std::endl;
// 				hit_color = inter.material->Kd * diff_intensity 
// 						  + inter.material->Ks * spec_intensity
// 						  + reflect_color * fres
// 						  + refract_color * (1 - fres);
// 				break;
// 			}
// 			case MaterialType::REFLECT : {
// 				vec3 pt = ray.at(inter.distance);
// 				vec3 normal = inter.normal.normalize();
// 				vec3 shadow_pt = dot(ray.dir, normal) > 0 ? pt - normal * 0.00001 : pt + normal * 0.00001;
// 				vec3 reflect_dir = reflect(normal, ray.dir);
// 				vec3 reflect_pt = dot(reflect_dir, normal) > 0 ? (pt + normal * 0.00001) : (pt - normal * 0.00001);
// 				color_t reflect_color = cast_ray(Ray(reflect_pt, reflect_dir), depth + 1);
// 				vec3 diff_intensity(0);
// 				vec3 spec_intensity(0);
// 				for (Light light: lights) {
// 					vec3 light_dir = light.pos - pt;
// 					float light_dist2 = light_dir.norm2();
// 					light_dir = light_dir.normalize();
// 					bool shadow = bvh.intersect(Ray(shadow_pt, light_dir)).happened;
// 					float factor = std::max(0.0, dot(light_dir, normal));
// 					diff_intensity += shadow ? vec3(0) : ((factor / light_dist2) * light.intensity);
// 					vec3 reflect_dir = reflect(normal, -light_dir);
// 					vec3 halfway_dir = ( -ray.dir + light_dir).normalize();
// 					factor = std::max(0.0, -dot(reflect_dir, ray.dir));
// 					spec_intensity += shadow ? vec3(0) : (pow(factor, inter.material->Ns) / light_dist2 * light.intensity);
// 				}
// 				float fres = fresnel(ray.dir, normal, inter.material->Ni);
// 				hit_color = inter.material->Kd * diff_intensity 
// 						  + inter.material->Ks * spec_intensity
// 						  + reflect_color * fres;
// 				// hit_color = (reflect_color )* fres;
// 				break;
// 			}
// 		}
// 	}
// 	return hit_color;
// }
// path tracing

vec3 Scene::cast_ray(const Ray &ray) {
	auto inter = intersect(ray);
	if (inter.happened) {
		if (inter.material->is_emission())
			return inter.material->emit;


		vec3 L_dir(0);
		vec3 L_indir(0);

		Intersection light_inter;
		float pdf_light;
		sample_light(light_inter, pdf_light);

		// vec3 pos = ray.at(inter.distance);
		vec3 pos = inter.pos;
		vec3 light_pos = light_inter.pos;

		vec3 N = inter.normal.normalize();	// object' surface normal
		vec3 NN = light_inter.normal.normalize();	// light' surface normal

		vec3 to_light_dir = (light_pos - pos).normalize();
		float to_light_dist = (light_pos - pos).norm2();
		Ray to_light(pos, to_light_dir);

		auto tmp = intersect(to_light);
		if (tmp.happened && (tmp.pos - light_pos).norm() < 0.01) {
			vec3 f_r = inter.material->eval(ray.dir, to_light_dir, N);		// the brdf value
			L_dir = light_inter.emit * f_r * dot(to_light_dir, N) * dot(-to_light_dir, NN) / to_light_dist / pdf_light;
		}

		if (random_float() < russian_roulette) {
			vec3 next_dir = inter.material->sample(ray.dir, N).normalize();
			Ray next_ray(pos, next_dir);
			Intersection next_inter = intersect(next_ray);
			if (next_inter.happened && !next_inter.material->is_emission()) {
				auto tmp = inter.material;
				float pdf = inter.material->pdf(ray.dir, next_dir, N);
				vec3 f_r = inter.material->eval(ray.dir, next_dir, N);
				L_indir = cast_ray(next_ray) * f_r * dot(next_dir, N) / pdf / russian_roulette;
			}
		}
		return L_dir + L_indir;
	}
	return vec3(0);
}

Intersection Scene::intersect(const Ray &ray) {
	return bvh.intersect(ray);
}


void Scene::sample_light(Intersection &inter, float &pdf_light) {
	float sum_area = 0;
	for (Object *obj: objs)
		if (obj->is_emmision())
			sum_area += obj->get_area();
	float p = sum_area * random_float();
	sum_area = 0;
	for (Object *obj : objs) {
		if (obj->is_emmision()) {
			sum_area += obj->get_area();
			if (sum_area >= p) {
				obj->sample(inter, pdf_light);
				break;
			}
		}
	}
}
