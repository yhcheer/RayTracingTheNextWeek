import java.util.ArrayList;
import java.util.List;

public class HitableList extends Hitable
{
    public List<Hitable> list;

    public HitableList() {
        list = new ArrayList<Hitable>();
    }
    public HitableList(List<Hitable> list) {
        this.list = list;
    }

    /**
     * 判断列表里的每个球是否撞击
     * @param r 光线
     * @param t_min 范围
     * @param t_max 范围
     * @param rec 撞击点
     * @return 任意一个球是否撞击
     */
    @Override
    public boolean hit(Ray r, float t_min, float t_max, HitRecord rec) {
        HitRecord tempRec = new HitRecord();
        boolean hitAnything = false;
        float closestSoFar = t_max;
        for(int i = 0; i < list.size(); i++)
        {
            if(list.get(i).hit(r, t_min, closestSoFar, tempRec))
            {
                hitAnything = true;
                closestSoFar = tempRec.t;       //每当有撞击点，tmax就会减为最近的一次撞击点
//                rec = tempRec;
                rec.t = tempRec.t;
                rec.normal = tempRec.normal;
                rec.p = tempRec.p;
                rec.matPtr = tempRec.matPtr;
            }
        }
        return hitAnything;
    }

    /**
     * 设置整个list包围盒
     * @param t0
     * @param t1
     * @param wrapper 包装传递box
     * @return 是否有包围盒
     */
    @Override
    public boolean bounding_box(float t0, float t1, Wrapper wrapper) {

        if (list.size() < 1) return false;
        Wrapper tempWrapper = new Wrapper();
        boolean first_true = list.get(0).bounding_box(t0, t1, tempWrapper); //传进去box参数
        if (!first_true)
            return false;
        else
            wrapper.box = tempWrapper.box;
        //遍历所有hitable,不断扩大box的范围
        for (int i = 1; i < list.size(); i++) {
            if(list.get(i).bounding_box(t0, t1, tempWrapper)) {
                wrapper.box = surrounding_box(wrapper.box, tempWrapper.box);
            }
        else
            return false;
        }
        return true;
    }

    /**
     * 两个包围盒形成一个更大的包围盒
     * @param box0 t0时刻的球
     * @param box1 t1时刻的球
     * @return 返回整个大的包围盒
     */
    public AABB surrounding_box(AABB box0, AABB box1){
        Vec3 small = new Vec3( Math.min(box0.min.x(), box1.min.x()), Math.min(box0.min.y(), box1.min.y()), Math.min(box0.min.z(), box1.min.z()));
        Vec3 big = new Vec3( Math.max(box0.max.x(), box1.max.x()), Math.max(box0.max.y(), box1.max.y()), Math.max(box0.max.z(), box1.max.z()));
        return new AABB(small, big);
    }
}