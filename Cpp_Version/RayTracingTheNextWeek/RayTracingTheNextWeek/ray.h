#pragma once

#include "vec3.h"

class ray
{
public:
	vec3 A;			// ����ԭ��
	vec3 B;			// ���߷���
	float _time;	// ���ߵ�ʱ���

	ray() {}
	ray(const vec3& a, const vec3& b) { A = a; B = b;}
	ray(const vec3& a, const vec3& b, const float & time) { A = a; B = b; _time = time; }

	/* ���ع���ԭ�� */
	vec3 origin() const { return A; }
	/* ���ع��߷��� */
	vec3 direction() const { return B; }
	/* ���ع���ʱ��� */
	float time() const { return  _time; };

	/* ���ع���λ��p(t)=A+t*B */
	vec3 point_at_parameter(float t) const { return A + t * B; }

	


};
