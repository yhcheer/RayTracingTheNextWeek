#pragma once
#include "hitable.h"

class moving_sphere :public  hitable
{
public:
	vec3 center(float time) const;
	float radius;
	material *mat_ptr;

	vec3 center0, center1;
	float time0, time1;

	moving_sphere() {}
	/*
		cen0 起始球心位置
		cen1 上限球心位置
		t0 开始时间
		t1 结束时间
		r 球半径
		m 球材质
	*/
	moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) :
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

	virtual  bool hit(const ray&r, float tmin, float tmax, hit_record& rec) const;
	bool bounding_box(float t0, float t1, aabb & box) const;
	aabb surrounding_box(aabb & box0, aabb & box1) const;
};

// 当前时间点，球心的位置
vec3 moving_sphere::center(float time) const {
	return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}



bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record & rec)const
{
	// 修改之前的center为一个时间相关的位置
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb& box) const {
	aabb box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
	aabb box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	box = surrounding_box(box0, box1);
	return true;
}

aabb moving_sphere::surrounding_box(aabb &box0, aabb &box1) const {
	vec3 small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));
	vec3 big(fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));
	return aabb(small, big);
}