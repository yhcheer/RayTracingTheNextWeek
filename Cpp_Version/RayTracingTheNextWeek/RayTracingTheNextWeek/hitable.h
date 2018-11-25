#pragma once

#include "ray.h"
#include "aabb.h"
#define M_PI 3.14159265358979323846

class material;

void get_sphere_uv(const vec3& p, float& u, float& v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}

struct hit_record
{
	float t;
	float u;// ƽ�������uvӳ��
	float v;// ƽ�������uvӳ��
	vec3 p;
	vec3 normal;
	material *mat_ptr;
};

class hitable {
public:
	/* �ж��Ƿ�ײ�� */
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	/* ���ð�Χ�� */
	virtual bool bounding_box(float t0, float t1, aabb & box) const = 0;
};

