#include "scene.h"
#include "gl.h"
#include "tgaimage.h"
#include "buffer.h"

void Scene::render() {
	buildBVH();

	ColorBuffer color_buf(width, height, color_t(0, 0, 0));
	vec3 eye_pos(-1, 5, 10);
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			float x = ((float)i + 0.5) - width / 2.f;
			float y = ((float)j + 0.5) - height / 2.f;
			float z = -height / 2 * tanf(fovY / 2.f);

			Ray ray(eye_pos, vec3(x, y, z));
			color_t color = cast_ray(ray, 0);
			color_buf.set(i, j, 0, color);
		}
	}

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

color_t Scene::cast_ray(const Ray &ray, int depth) {
	if (depth > 4)
		return color_t(0.2, 0.6, 0.8);
	
	auto inter = intersect(ray);
	color_t hit_color(0.2, 0.6, 0.8);
	if (inter.happened) {
		switch (inter.material->m_type)
		{
			case MaterialType::DIFFUSE_GLOSSY : {

				vec3 pt = ray.at(inter.distance);
				vec3 normal = inter.normal.normalize();
				vec3 shadow_pt = dot(ray.dir, normal) > 0 ? pt - normal * 0.00001 : pt + normal * 0.00001;

				vec3 diff_intensity(0);
				vec3 spec_intensity(0);
				for (Light light: lights) {
					vec3 light_dir = (light.pos - pt).normalize();
					bool shadow = bvh.intersect(Ray(shadow_pt, light_dir)).happened;

					// float tmp = dot(light_dir, normal);
					// std::cout << tmp << std::endl;
					float factor = std::max(0.0, dot(light_dir, normal));
					diff_intensity += shadow ? vec3(0) : (factor * light.intensity);
					vec3 reflect_dir = reflect(normal, -light_dir);
					vec3 halfway_dir = ( -ray.dir + light_dir).normalize();
					factor = std::max(0.0, -dot(reflect_dir, ray.dir));
					spec_intensity += shadow ? vec3(0) : (pow(factor, inter.material->Ns) * light.intensity);
				}

				hit_color =  inter.material->Kd * diff_intensity; 
				break;
			}
			case MaterialType::REFLECT_REFRACT : {
				vec3 pt = ray.at(inter.distance);
				vec3 normal = inter.normal.normalize();

				vec3 reflect_dir = reflect(normal, ray.dir);
				vec3 reflect_pt = dot(reflect_dir, normal) > 0 ? (pt + normal * 0.00001) : (pt - normal * 0.00001);
				color_t reflect_color = cast_ray(Ray(reflect_pt, reflect_dir), depth + 1);

				auto refract_dir = refract(normal, ray.dir, inter.material->Ni);
				color_t refract_color = {0, 0, 0};
				if (refract_dir.has_value()) {
					vec3 refract_pt = dot(refract_dir.value(), normal) > 0 ? (pt + normal * 0.00001) : (pt - normal * 0.00001);
					refract_color = cast_ray(Ray(pt, refract_dir.value()), depth + 1);
				}
				float fres = fresnel(ray.dir, inter.normal, inter.material->Ni);
				hit_color = reflect_color * fres + refract_color * (1 - fres);
				break;
			}
			case MaterialType::REFLECT : {
				vec3 pt = ray.at(inter.distance);
				vec3 normal = inter.normal.normalize();

				vec3 reflect_dir = reflect(normal, ray.dir);
				vec3 reflect_pt = dot(reflect_dir, normal) > 0 ? (pt + normal * 0.00001) : (pt - normal * 0.00001);
				color_t reflect_color = cast_ray(Ray(reflect_pt, reflect_dir), depth + 1);

				float fres = fresnel(ray.dir, normal, inter.material->Ni);
				hit_color = reflect_color * fres;
				break;
			}
		}
	}
	return hit_color;
}

Intersection Scene::intersect(const Ray &ray) {
	return bvh.intersect(ray);
}