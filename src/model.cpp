#include <iostream>
#include <sstream>
#include "model.h"

Model::Model(const std::string filename) {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail())
		return;
	std::string line;
	std::string s;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			vec3 v;
			for (int i = 0; i < 3; ++i)
				iss >> v[i];
			verts.push_back(v);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			vec2 vt;
			for (int i = 0; i < 2; ++i)
				iss >> vt[i];
			tex_coord.push_back(vt);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			vec3 vn;
			for (int i = 0; i < 3; ++i)
				iss >> vn[i];
			norms.push_back(vn);
		}
		else if (!line.compare(0, 2, "f ")) {
			iss >> trash;
			int v, vt, vn;
			int cnt = 0;

			while (iss >> s) {
				size_t p1 = s.find_first_of('/');
				size_t p2 = s.find_last_of('/');
				if (p1 + 1 < p2 && p1 != std::string::npos) {		// f v/vt/vn
					v = std::stoi(s.substr(0, p1));
					vt = std::stoi(s.substr(p1 + 1, p2 - p1 - 1));
					vn = std::stoi(s.substr(p2 + 1, s.size() - p2 - 1));

					facet_vrt.push_back(--v);
					facet_tex.push_back(--vt);
					facet_nrm.push_back(--vn);
				} else if (p1 + 1 == p2) {		// f v//vn
					v = std::stoi(s.substr(0, p1));
					vn = std::stoi(s.substr(p2 + 1, s.size() - p2 - 1));

					facet_vrt.push_back(--v);
					facet_nrm.push_back(--vn);
				} else if (p1 == p2 && p1 != std::string::npos) {	// f v/vt
					v = std::stoi(s.substr(0, p1));
					vt = std::stoi(s.substr(p2 + 1, s.size() - p2 - 1));

					facet_vrt.push_back(--v);
					facet_tex.push_back(--vt);
				} else if (p1 == p2 && p1 == std::string::npos) {	// f v
					v = std::stoi(s);
					facet_vrt.push_back(--v);
				}
				++cnt;
			}

			if (3 != cnt) {
				std::cerr << "Error: the obj file is supposed to be trangulated" << std::endl; 
				in.close(); 
				return;
			}
		}
	}
	if (norms.size() == 0)
		gen_normal();
	in.close();
}

int Model::nverts() const {
	return verts.size();
}

int Model::nfaces() const {
	return facet_vrt.size() / 3;
}

vec3 Model::normal(const int iface, const int nthvert) const {
	return norms[facet_nrm[iface * 3 + nthvert]];
}


vec3 Model::vert(const int i) const {
	return verts[i];
}

vec3 Model::vert(const int iface, const int nthvert) const {
	return verts[facet_vrt[iface * 3 + nthvert]];
}

vec2 Model::uv(const int iface, const int nthvert) const {
	return tex_coord[facet_tex[iface * 3 + nthvert]];
}

bool Model::intersect(const Ray &ray, float &near, vec3 &nrm) {
	float dist = std::numeric_limits<float>::max();
	for (int i = 0; i < nfaces(); ++i) {
		vec3 pts[3];
		vec3 norms[3];
		for (int j = 0; j < 3; ++j) {
			pts[j] = vert(i, j);
			norms[j] = normal(i, j);
		}
		Triangle tri(pts);
		vec3 bar;
		if (tri.intersect(ray, bar, near) && near < dist) {
			dist = near;
			nrm = bar.x * norms[0] + bar.y * norms[1] + bar.z * norms[2];
		}
	}
	near = dist;
	if (dist == std::numeric_limits<float>::max())
		return false;
	else
		return true;
}


void Model::gen_normal() {
	facet_nrm = facet_vrt;
	norms.assign(nverts(), vec3(0, 0, 0));
	vec3 pts[3];
	std::vector<float> tot_areas(nverts(), 0);
	for (int i = 0; i < nfaces(); ++i) {
		for (int j = 0; j < 3; ++j) {
			pts[j] = vert(i, j);
		}
		float cur_area = area(pts);
		vec3  cur_normal = cross(pts[1] - pts[0], pts[2] - pts[0]).normalize();
		for (int j = 0; j < 3; ++j) {
			int idx = facet_vrt[i * 3 + j];
			tot_areas[idx] += cur_area;
			norms[idx] = norms[idx] + cur_normal * cur_area;
		}
	}
	for (int i = 0; i < nverts(); ++i) {
		norms[i] = norms[i] / tot_areas[i];
	}
}