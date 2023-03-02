#include <iostream>
#include "global.h"
#include "buffer.h"
#include "gl.h"
#include "ray.h"
#include "model.h"
#include "tgaimage.h"
#include "camera.h"

struct Material {
    float refract_index = 1;
    float albedo[4] = {2,0,0,0};
    color_t diff_color = {0,0,0};
    float spec_exponent = 0;
};

const Material      ivory = {1.0, {0.9,  0.5, 0.1, 0.0}, color_t(0.4, 0.4, 0.3),   50.};
const Material      glass = {1.5, {0.0,  0.9, 0.1, 0.8}, color_t(0.6, 0.7, 0.8),  125.};
const Material red_rubber = {1.0, {1.4,  0.3, 0.0, 0.0}, color_t(0.3, 0.1, 0.1),   10.};
const Material     mirror = {1.0, {0.0, 16.0, 0.8, 0.0}, color_t(1.0, 1.0, 1.0), 1425.};
// const Material      glass = {1.5, {0.0,  0.9, 0.1, 0.8}, color_t{0.6, 0.7, 0.8},  125.};
// const Material red_rubber = {1.0, {1.4,  0.3, 0.0, 0.0}, color_t(0.3, 0.1, 0.1),   10.};

vec3 light_pos(0, 0, 3);

color_t cast_ray(Model *model, const Ray& ray, int depth = 0) {
	float near = std::numeric_limits<float>::max();
	vec3 normal;
	if (!model->intersect(ray, near, normal) || depth == 4)
		return color_t(0.2, 0.7, 0.8);
	

	vec3 pt = ray.at(near);
	normal = normal.normalize();
	if (dot(ray.dir, normal) < 0) {
		pt = pt + normal * 0.0001;
	}
	else {
		pt = pt - normal * 0.0001;
	}

	Ray reflect_ray(pt, reflect(normal, ray.dir));
	color_t reflect_color = cast_ray(model, reflect_ray, depth + 1);

	Ray refract_ray;
	color_t refract_color(0, 0, 0);
	auto refract_dir = refract(normal, ray.dir, 1, red_rubber.refract_index);
	if (refract_dir.has_value()) {
		refract_ray = Ray(pt, refract_dir.value());
		refract_color = cast_ray(model, refract_ray, depth + 1);
	}

	Ray light_ray(light_pos, pt - light_pos);
	float dist = std::numeric_limits<float>::max();
    float diffuse_light_intensity = 0, specular_light_intensity = 0;

	Material m = red_rubber; 

	if (model->intersect(light_ray, dist, normal)) {
		vec3 light_pt = light_ray.at(dist);
		if ((light_pos - light_pt).norm2() + 0.001 >= (light_pos - pt).norm2()) {
			diffuse_light_intensity = std::max(0.0, dot(-light_ray.dir, normal));
			vec3 half_way = -(ray.dir + light_ray.dir);
	        // specular_light_intensity = std::pow(std::max(0.0, dot(half_way, normal)), red_rubber.spec_exponent);
	        specular_light_intensity = std::pow(std::max(0.0, dot(-reflect(-light_ray.dir, normal), ray.dir)), m.spec_exponent);
		}
	}
    color_t c1 = m.diff_color * diffuse_light_intensity * m.albedo[0];
    color_t c2 = reflect_color* m.albedo[2];
	color_t c3 = refract_color* m.albedo[3];
    color_t c4 = color_t{1., 1., 1.}*specular_light_intensity * m.albedo[1];


	color_t ret = c1 + c2 + c3 + c4;
	return ret;
}

int main() {
	Model *cube = new Model("../obj/cube/cube.obj");

	constexpr int width = 800;
	constexpr int height = 800;
	float fovY = radius(60);
	ColorBuffer color_buf(width, height, color_t(0, 0, 0));

	vec3 eye(0, 0, 1);
	float z = -height / 2.f * tanf(fovY / 2.f);
#pragma omp parallel for
	for (int i = 0; i < width; ++i) {
#pragma omp parallel for
		for (int j = 0; j < height; ++j) {
			float x = (i + 0.5) - width / 2.f;
			float y = (j + 0.5) - height / 2.f;

			Ray ray(eye, vec3(x, y, z) - eye);
			color_t c = cast_ray(cube, ray);
			color_buf.set(i, j, 0, c);
		}
	}

	TGAImage image(width, height, TGAImage::RGB);
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			image.set(x, y, color_buf.get_value(x, y));
		}
	}

	image.write_tga_file("output.tga");
	system("convert output.tga output.png");
	system("mv output.png ../");

	return 0;
}