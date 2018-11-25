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
	float u;// 平面坐标的uv映射
	float v;// 平面坐标的uv映射
	vec3 p;
	vec3 normal;
	material *mat_ptr;
};

class hitable {
public:
	/* 判断是否撞击 */
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	/* 设置包围盒 */
	virtual bool bounding_box(float t0, float t1, aabb & box) const = 0;
};

