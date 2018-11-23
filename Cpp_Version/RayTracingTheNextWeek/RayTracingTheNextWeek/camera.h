#pragma once

#include "ray.h"

#define M_PI 3.14159265358979323846

/* 生成单位随机圆 */
vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0 * vec3((rand() % 10000 / 10000.0), (rand() % 10000 / 10000.0), 0) - vec3(1, 1, 0);	// 保证范围在[-1. 1）
	} while (dot(p, p) >= 1.0);
	return p;
}

class camera {
public:
	vec3 origin;			// 相机原点
	vec3 lower_left_corner;	// 左下角原点
	vec3 horizontal;		// 宽
	vec3 vertical;			// 高
	vec3 u, v, w;			// 相机xyz轴
	float lens_radius;		// 光圈半径
	
	float time0;			// 快门开启的时间
	float time1;			// 快门关闭的时间

	/*
		初始化相机信息
		@param lookfrom 相机位置
		@param lookat 观察点
		@param vup 相机倾斜角，一般取(0，1，0)方向
		@param vfov 视野 (field of view)
		@param aspect 宽高比
		@param aperture 光圈大小
		@param focus_dist 聚焦距离
		@paramt t0 快门开启
		@paramt t1 快门关闭
	*/
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) {
		time0 = t0;
		time1 = t1;
		lens_radius = aperture / 2;

		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - 1 * w * focus_dist;	// (- half_width, - half_height, -1)
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;
	}

	/* 根据uv坐标 得到光线 */
	ray get_ray(float s, float t) {
		vec3 rd = lens_radius * random_in_unit_disk();	// 模拟光圈棱镜
		vec3 offset = u * rd.x() + v * rd.y();			// 偏移
		float time = time0 + rand() % 10000 / 10000.0 * (time1 - time0);	// 在快门开启的时间段内 随机的一段时间
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - (origin + offset), time); // 经过光圈后 原点偏移了
	}

};

