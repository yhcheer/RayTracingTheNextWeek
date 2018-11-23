#pragma once

#include "ray.h"

#define M_PI 3.14159265358979323846

/* ���ɵ�λ���Բ */
vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0 * vec3((rand() % 10000 / 10000.0), (rand() % 10000 / 10000.0), 0) - vec3(1, 1, 0);	// ��֤��Χ��[-1. 1��
	} while (dot(p, p) >= 1.0);
	return p;
}

class camera {
public:
	vec3 origin;			// ���ԭ��
	vec3 lower_left_corner;	// ���½�ԭ��
	vec3 horizontal;		// ��
	vec3 vertical;			// ��
	vec3 u, v, w;			// ���xyz��
	float lens_radius;		// ��Ȧ�뾶
	
	float time0;			// ���ſ�����ʱ��
	float time1;			// ���Źرյ�ʱ��

	/*
		��ʼ�������Ϣ
		@param lookfrom ���λ��
		@param lookat �۲��
		@param vup �����б�ǣ�һ��ȡ(0��1��0)����
		@param vfov ��Ұ (field of view)
		@param aspect ��߱�
		@param aperture ��Ȧ��С
		@param focus_dist �۽�����
		@paramt t0 ���ſ���
		@paramt t1 ���Źر�
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

	/* ����uv���� �õ����� */
	ray get_ray(float s, float t) {
		vec3 rd = lens_radius * random_in_unit_disk();	// ģ���Ȧ�⾵
		vec3 offset = u * rd.x() + v * rd.y();			// ƫ��
		float time = time0 + rand() % 10000 / 10000.0 * (time1 - time0);	// �ڿ��ſ�����ʱ����� �����һ��ʱ��
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - (origin + offset), time); // ������Ȧ�� ԭ��ƫ����
	}

};

