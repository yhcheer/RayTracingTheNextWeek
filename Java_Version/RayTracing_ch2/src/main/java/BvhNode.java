import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class BvhNode extends Hitable {

    Hitable left;
    Hitable right;
    AABB box;

//    List<Hitable> list = new ArrayList<Hitable>();
    public BvhNode() {
    }

    /**
     *
     * @param list 物体数组
     * @param n 物体数组长度
     * @param time0
     * @param time1
     */
    public BvhNode(List<Hitable> list, int n, float time0, float time1) {

        int axis = (int)( 3 * Math.random() );  //随机选择一个轴0 1 2
        //对图元进行排序
        if(axis == 0) {
            Collections.sort(list, new Box_x_compare());
        }
        else if(axis == 1){
            Collections.sort(list, new Box_y_compare());
        }
        else {
            Collections.sort(list, new Box_z_compare());
        }
        //递归出口
        if( n == 1 ){
            left = list.get(0);
        }
        else if( n == 2 ){
            left = list.get(0);
            right = list.get(1);
        }
        else{
            left = new BvhNode(list.subList(0, n/2), n/2 , time0, time1);
            right = new BvhNode(list.subList(n/2, n - 1), n - n/2, time0, time1);
        }
        Wrapper box_left = new Wrapper(), box_right = new Wrapper();
        if(!left.bounding_box(time0, time1, box_left) || !right.bounding_box(time0, time1, box_left)){
            System.out.println("No bounding box in bvh_node constructor.");
        }
        box = surrounding_box(box_left.box, box_right.box);
    }

    class Box_x_compare implements Comparator{


        public int compare(Object o1, Object o2) {
            Wrapper box_left = new Wrapper();
            Wrapper box_right = new Wrapper();
            Hitable ah = (Hitable)o1;
            Hitable bh = (Hitable)o2;
            if(!ah.bounding_box(0,0, box_left) || !bh.bounding_box(0,0, box_right))
                System.out.println("no bounding box in bvh_node constructor.");
            if(box_left.box.min.x() - box_right.box.min.x()<0.0)
                return  -1;
            else
                return 1;
        }
    }
    class Box_y_compare implements Comparator{
        public int compare(Object o1, Object o2) {
            Wrapper box_left = new Wrapper();
            Wrapper box_right = new Wrapper();
            Hitable ah = (Hitable)o1;
            Hitable bh = (Hitable)o2;
            if(!ah.bounding_box(0,0, box_left) || !bh.bounding_box(0,0, box_right))
                System.out.println("no bounding box in bvh_node constructor.");
            if(box_left.box.min.y() - box_right.box.min.y()<0.0)
                return  -1;
            else
                return 1;
        }
    }
    class Box_z_compare implements Comparator{
        public int compare(Object o1, Object o2) {
            Wrapper box_left = new Wrapper();
            Wrapper box_right = new Wrapper();
            Hitable ah = (Hitable)o1;
            Hitable bh = (Hitable)o2;
            if(!ah.bounding_box(0,0, box_left) || !bh.bounding_box(0,0, box_right))
                System.out.println("no bounding box in bvh_node constructor.");
            if(box_left.box.min.z() - box_right.box.min.z()<0.0)
                return  -1;
            else
                return 1;
        }
    }




    /**
     * 判断光线和BVH层次包围盒是否有碰撞
     * @param r 光线
     * @param t_min 光线开始时间
     * @param t_max 光线结束时间
     * @param rec 碰撞点
     * @return 是否碰撞
     */
    @Override
    public boolean hit(Ray r, float t_min, float t_max, HitRecord rec) {
        if(box.hit(r, t_min, t_max)){
            //光线有碰撞 左右子树
            HitRecord left_rec = new HitRecord();
            HitRecord right_rec = new HitRecord();
            boolean hitLeft = left.hit(r, t_min, t_max, left_rec);
            boolean hirRight = right.hit(r, t_min, t_max, right_rec);
            if(hitLeft && hirRight){
                if(left_rec.t < right_rec.t){
                    //rec = left_rec;
                    rec.t = left_rec.t;
                    rec.p = left_rec.p;
                    rec.normal = left_rec.normal;
                    rec.matPtr = left_rec.matPtr;
                    return true;
                }
                else{
                    rec.t = right_rec.t;
                    rec.p = right_rec.p;
                    rec.normal = right_rec.normal;
                    rec.matPtr = right_rec.matPtr;
                    return true;
                }
            }else if(hitLeft){
                rec.t = left_rec.t;
                rec.p = left_rec.p;
                rec.normal = left_rec.normal;
                rec.matPtr = left_rec.matPtr;
                return true;
            }else if(hirRight){
                rec.t = right_rec.t;
                rec.p = right_rec.p;
                rec.normal = right_rec.normal;
                rec.matPtr = right_rec.matPtr;
                return true;
            }else {
                return false;
            }
        }
        else{
            return false;
        }
    }

    @Override
    public boolean bounding_box(float t0, float t1, Wrapper wrapper) {
        wrapper.box = this.box;
        return true;
    }
}
