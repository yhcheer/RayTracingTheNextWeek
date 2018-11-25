#pragma GCC optimize(3)
// RayTracingInOneWeekend.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>
#include <fstream>
#include "sphere.h"
#include "bvh.h"
#include "moving_sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include <math.h>
#include <time.h> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "image_texture.h"



#define MAXFLOAT 3.40282e+038

using namespace std;


vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		//return vec3(0, 0, 0);
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable *earth() {
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_data, nx, ny));
	return new sphere(vec3(0, 0, 0), 2, mat);
}

hitable *random_scene2() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	texture *checkerTexture = new checker_texture(new constant_texture(vec3(0.37, 0.22, 0.06)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	texture *constantTexture = new constant_texture(vec3(Rand()*Rand(), Rand()*Rand(), Rand()*Rand()));
	texture *noiseTexture = new noise_texture(4);
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	int nx2, ny2, nn2;
	unsigned char *tex_data2 = stbi_load("TexturesCom_MetalBare0181_16_seamless_S.jpg", &nx2, &ny2, &nn2, 0);
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(new image_texture(tex_data, nx, ny)));
	int i = 1;
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new lambertian(new image_texture(tex_data2, nx2, ny2)));
	
	return new bvh_node(list, i, 0.0, 1.0);
}

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	texture *checkerTexture = new checker_texture(new constant_texture(vec3(0.37, 0.22, 0.06)),new constant_texture(vec3(0.9, 0.9, 0.9)));
	texture *constantTexture = new constant_texture(vec3(Rand()*Rand(), Rand()*Rand(), Rand()*Rand()));
	texture *noiseTexture = new noise_texture(4);

	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(noiseTexture));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = Rand();
			vec3 center(a + 0.9*Rand(), 0.2, b + 0.9*Rand());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					// 运动模糊的小球
					list[i++] = new moving_sphere(
						center, 
						center + vec3(0, 0.5 * Rand(), 0), 
						0.0,
						1.0, 
						0.2,
						new lambertian(new constant_texture(vec3(Rand()*Rand(), Rand()*Rand(), Rand()*Rand())))
					);
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + Rand()), 0.5*(1 + Rand()), 0.5*(1 + Rand())), 0.5*Rand()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	//list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	//list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(Rand()*Rand(), Rand()*Rand(), Rand()*Rand()))));
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new lambertian(noiseTexture));
	//list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	//return new hitable_list(list, i);
	return new bvh_node(list, i, 0.0, 1.0);
}

int main() {
	int nx = 1980;
	int ny = 1080;
	int ns = 100;
	srand((unsigned)time(NULL));
	ofstream outfile("Raytracing_ch5_2.ppm", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	float R = cos(M_PI / 4);
	hitable *world = random_scene2();
	//hitable * world = random_scene();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.1;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus, 0, 1);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + Rand()) / float(nx);
				float v = float(j + Rand()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
		std::cout << j << "\n";
	}
}




