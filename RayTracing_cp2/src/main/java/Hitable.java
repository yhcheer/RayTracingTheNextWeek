public abstract class Hitable
{
    public abstract boolean hit(final Ray r, float t_min, float t_max, HitRecord rec);

    public abstract boolean bounding_box(float t0, float t1, Wrapper wrapper);

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
