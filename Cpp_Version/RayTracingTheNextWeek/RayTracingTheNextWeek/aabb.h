#pragma once

#include "vec3.h"
#include "ray.h"

// aabb包围盒
class aabb
{
public:
	vec3 _min;
	vec3 _max;

	aabb() {}
	aabb(const vec3 a, const vec3 &b)
	{
		_min = a; _max = b;
	}

	vec3 min()const { return  _min; };
	vec3 max()const { return  _max; };

	/* 判断AABB盒是否碰撞 */
	bool hit(const ray& r, float tmin, float tmax)const
	{
		for (int a = 0; a < 3; a++)	// 三个维度
		{
			// t = (p - A) / B
			float invD = 1.0f / r.direction()[a];	// 1/B
			float t0 = (min()[a] - r.origin()[a]) * invD;	
			float t1 = (max()[a] - r.origin()[a]) * invD;	
			
			if (t0 > t1)
				std::swap(t0, t1);
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax <= tmin)
				return false;
		}
		return true;
	}
};