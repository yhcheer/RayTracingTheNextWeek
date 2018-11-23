#pragma once

#include "vec3.h"

class ray
{
public:
	vec3 A;			// 光线原点
	vec3 B;			// 光线方向
	float _time;	// 光线的时间戳

	ray() {}
	ray(const vec3& a, const vec3& b) { A = a; B = b;}
	ray(const vec3& a, const vec3& b, const float & time) { A = a; B = b; _time = time; }

	/* 返回光线原点 */
	vec3 origin() const { return A; }
	/* 返回光线方向 */
	vec3 direction() const { return B; }
	/* 返回光线时间戳 */
	float time() const { return  _time; };

	/* 返回光线位置p(t)=A+t*B */
	vec3 point_at_parameter(float t) const { return A + t * B; }

	


};
