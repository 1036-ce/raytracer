#include "scene.h"
#include "gl.h"
#include "tgaimage.h"
#include "buffer.h"
#include "camera.h"

void Scene::render() {
	// for (auto obj: objs)
	// 	std::cout << obj->get_area() << std::endl;
	buildBVH();

	ColorBuffer color_buf(width, height, color_t(0, 0, 0));
    float scale = tan((fovY * 0.5));
    float aspect= width / (float)height;
	Point3 eye_pos(278, 273, -800);

	int spp = 2048;

	std::vector<std::pair<float, float>> offsets;
	for (int i = 0; i < spp; ++i)
		offsets.emplace_back(random_float(), random_float());
	
	for (int j = 0; j < height; ++j) {
		#pragma omp parallel for
		for (int i = 0; i < width; ++i) {
			float x = i - (float)width / 2.f;
			float y = j - (float)height / 2.f;
			float z = (float)height / (2 * scale);
			vec3 tmp(0);
			for (int k = 0; k < spp; k++){
				float nx = x + offsets[k].first;
				float ny = y + offsets[k].second;
				Ray ray(eye_pos, vec3(-nx, ny, z).normalize());
				tmp += (cast_ray(ray) / spp);
            }
			color_t color(tmp.x, tmp.y, tmp.z);
			color_buf.set(i, j, 0, color);
		}
		update_progress((float)j / height);
	}
	update_progress(1.0);

	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			color_t c = color_buf.get_value(i, j);
			c.r = std::pow(std::clamp<float>(c.r, 0, 1), 0.6f);
			c.g = std::pow(std::clamp<float>(c.g, 0, 1), 0.6f);
			c.b = std::pow(std::clamp<float>(c.b, 0, 1), 0.6f);
			c.a = std::clamp<float>(c.a, 0, 1);
			image.set(i, j, c);
		}
	}
	image.write_tga_file("output.tga");
	system("convert output.tga output.png");
	system("mv output.png ../");
}

void Scene::buildBVH() {
	bvh.root = bvh.build(objs);
}


vec3 Scene::cast_ray(const Ray &ray, int depth) {
	auto inter = intersect(ray);
	if (inter.happened) {
		if (inter.material->type() == MaterialType::EMITTER)
			return inter.material->emit();
			// return inter.material->eval(vec3(), -ray.dir, inter.normal);
		// if (inter.material->is_emission())
			// return inter.material->emit;


		vec3 L_dir(0);
		vec3 L_indir(0);

		Intersection light_inter;
		float pdf_light;
		sample_light(light_inter, pdf_light);

		// vec3 pos = ray.at(inter.distance);
		Point3 pos = inter.pos;

		Point3 light_pos = light_inter.pos;

		vec3 N = inter.normal.normalize();	// object' surface normal
		vec3 NN = light_inter.normal.normalize();	// light' surface normal

		pos = (dot(ray.dir, N) < 0) ?  (pos + N * 0.0001) : (pos - N * 0.0001);


		vec3 to_light_dir = (light_pos - pos).normalize();
		float to_light_dist2 = (light_pos - pos).norm2();
		Ray to_light(pos, to_light_dir);

		auto tmp = intersect(to_light);
		if (inter.material->type() != MaterialType::REFLECT && (tmp.pos - light_pos).norm() < 0.01) {
			vec3 f_r = inter.material->eval(ray.dir, to_light_dir, N);	
			vec3 tmp = light_inter.material->emit() * dot(-to_light_dir, NN);
			L_dir = tmp * f_r * dot(to_light_dir, N)  / (to_light_dist2 * pdf_light);
		}

		if (random_float() < russian_roulette) {
			vec3 next_dir = inter.material->sample(ray.dir, N).normalize();

			vec3 v = (next_dir + ray.dir).normalize();
			float t = dot(v, N);

			Ray next_ray(pos, next_dir);
			Intersection next_inter = intersect(next_ray);
			if (inter.material->type() == MaterialType::REFLECT) {
				if (next_inter.happened) {
					float pdf = inter.material->pdf(ray.dir, next_dir, N);
					vec3 f_r = inter.material->eval(ray.dir, next_dir, N);
					vec3 nxt = cast_ray(next_ray, depth + 1);
					float c = std::max(dot(next_dir, N), 0.0f);
					L_indir = (nxt * f_r * c) / (pdf * russian_roulette);
				}
			}
			else {
				if (next_inter.happened && next_inter.material->type() != MaterialType::EMITTER) {
					float pdf = inter.material->pdf(ray.dir, next_dir, N);
					vec3 f_r = inter.material->eval(ray.dir, next_dir, N);
					vec3 nxt = cast_ray(next_ray, depth + 1);
					float c = std::max(dot(next_dir, N), 0.f);
					L_indir = (nxt * f_r * c) / (pdf * russian_roulette);
				}
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
		// if (obj->)
			sum_area += obj->get_area();
			if (sum_area >= p) {
				obj->sample(inter, pdf_light);
				break;
			}
		}
	}
}
