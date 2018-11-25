#pragma once

#include "hitable.h"

class bvh_node :public hitable
{
public:
	hitable *left;	// ��������ַ
	hitable *right;	// ��������ַ
	aabb box;		// AABB��Χ��

	bvh_node() {}
	bvh_node(hitable **l, int n, float time0, float time1);

	virtual bool hit(const ray&r, float tmin, float tmax, hit_record &rec)const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;

	aabb surrounding_box(aabb & box0, aabb & box1) const;

};


/* ������ȡ��Χ�� */
bool bvh_node::bounding_box(float t0, float t1, aabb& b) const {
	b = box;
	return true;
}

/* ���ܰ�Χ�� */
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
	��ʼ��BVH�� 
	hitable **l BVH����ַ
	int n BVH���ڽڵ�����
	float time0 
	float time1
*/
bvh_node::bvh_node(hitable **l, int n, float time0, float time1) {
	int axis = int(3 * Rand());		// ���ѡһ����
	if (axis == 0)
		qsort(l, n, sizeof(hitable *), box_x_compare);
		//1 �����������׵�ַ 2 �����д�����Ԫ������ 3 ��Ԫ�ص�ռ�ÿռ��С 4 ָ������ָ��
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

/* �жϰ�Χ���Ƿ���ײ */
bool bvh_node::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {
	if (box.hit(r, tmin, tmax))	
	{
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, tmin, tmax, left_rec);		// �������Ƿ�����ײ����������ײ��Ϣ
		bool hit_right = right->hit(r, tmin, tmax, right_rec);	// �������Ƿ�����ײ����������ײ��Ϣ
		if (hit_left && hit_right)          // �����ص�����
		{
			if (left_rec.t < right_rec.t)
				rec = left_rec;             // ����������
			else
				rec = right_rec;            // ����������
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
		return false;                       // δ�����κ�����
}

