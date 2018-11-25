#pragma once

#include "hitable.h"

class bvh_node :public hitable
{
public:
	hitable *left;	// 左子树地址
	hitable *right;	// 右子树地址
	aabb box;		// AABB包围盒

	bvh_node() {}
	bvh_node(hitable **l, int n, float time0, float time1);

	virtual bool hit(const ray&r, float tmin, float tmax, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;

	aabb surrounding_box(aabb & box0, aabb & box1) const;

};


/* 用来获取包围盒 */
bool bvh_node::bounding_box(float t0, float t1, aabb& b) const {
	b = box;
	return true;
}

/* 重塑包围盒 */
aabb bvh_node::surrounding_box(aabb &box0, aabb &box1) const {
	vec3 small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));
	vec3 big(fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));
	return aabb(small, big);
}

int box_x_compare(const void * a, const void * b) {
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	if (box_left.min().x() - box_right.min().x() < 0.0)
		return -1;
	else
		return 1;
}
int box_y_compare(const void * a, const void * b)
{
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	if (box_left.min().y() - box_right.min().y() < 0.0)
		return -1;
	else
		return 1;
}
int box_z_compare(const void * a, const void * b)
{
	aabb box_left, box_right;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	if (box_left.min().z() - box_right.min().z() < 0.0)
		return -1;
	else
		return 1;
}

/* 
	初始化BVH树 
	hitable **l BVH树地址
	int n BVH树内节点总数
	float time0 
	float time1
*/
bvh_node::bvh_node(hitable **l, int n, float time0, float time1) {
	int axis = int(3 * Rand());		// 随机选一个轴
	if (axis == 0)
		qsort(l, n, sizeof(hitable *), box_x_compare);
		//1 待排序数组首地址 2 数组中待排序元素数量 3 各元素的占用空间大小 4 指向函数的指针
	else if (axis == 1)
		qsort(l, n, sizeof(hitable *), box_y_compare);
	else
		qsort(l, n, sizeof(hitable *), box_z_compare);

	if (n == 1) {
		left = right = l[0];
	}
	else if (n == 2) {
		left = l[0];
		right = l[1];
	}
	else {
		left = new bvh_node(l, n / 2, time0, time1);
		right = new bvh_node(l + n / 2, n - n / 2, time0, time1);
	}

	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
		std::cerr << "no bounding box in bvh_node constructor\n";
	box = surrounding_box(box_left, box_right);
}

/* 判断包围盒是否碰撞 */
bool bvh_node::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {
	if (box.hit(r, tmin, tmax))	
	{
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, tmin, tmax, left_rec);		// 左子树是否有碰撞，并更新碰撞信息
		bool hit_right = right->hit(r, tmin, tmax, right_rec);	// 右子树是否有碰撞，并更新碰撞信息
		if (hit_left && hit_right)          // 击中重叠部分
		{
			if (left_rec.t < right_rec.t)
				rec = left_rec;             // 击中左子树
			else
				rec = right_rec;            // 击中右子树
			return true;
		}
		else if (hit_left)
		{
			rec = left_rec;
			return  true;
		}
		else if (hit_right)
		{
			rec = right_rec;
			return true;
		}
		else
			return false;
	}
	else
		return false;                       // 未击中任何物体
}

