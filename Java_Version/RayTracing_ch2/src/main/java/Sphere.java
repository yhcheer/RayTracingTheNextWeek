public class Sphere extends Hitable
{
    Vec3 center;
    float radius;
    Material mat;

    public Sphere() { }
    public Sphere(Vec3 center, float radius, Material mat) {
        this.center = center;
        this.radius = radius;
        this.mat = mat;
    }

    /**
     * 判断与球体是否有撞击
     * @param r 光线
     * @param t_min 光线开始时间
     * @param t_max 光线结束时间
     * @param rec 撞击点
     * @return 是否有撞击
     */
    @Override
    public boolean hit(Ray r, float t_min, float t_max, HitRecord rec) {
        Vec3 oc = r.origin().Subtract(center);
        float a = r.direction().dot(r.direction());
        float b = 2.0f * oc.dot(r.direction());
        float c = oc.dot(oc) - radius*radius;
        float discriminant = b*b - 4.0f*a*c;
        if(discriminant > 0)
        {
            //优先选取符合范围的根较小的撞击点，若没有再选取另一个根
            float discFactor = (float)Math.sqrt(discriminant);
            float temp = (-b - discFactor) / (2.0f*a);
            if(temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p.Subtract(center)).Scale(1.0f/radius);
                rec.matPtr = mat;
                return true;
            }
            temp = (-b + discFactor) / (2.0f*a);
            if(temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p.Subtract(center)).Scale(1.0f/radius);
                rec.matPtr = mat;
                return true;
            }
        }
        return false;
    }

    /**
     * 静态的球的包围盒
     * @param t0
     * @param t1
     * @param wrapper 传box
     * @return 返回 球心-(R,R,R) -> 球心+(R,R,R) 区间的AABB
     */
    @Override
    public boolean bounding_box(float t0, float t1, Wrapper wrapper) {
        wrapper.box = new AABB(center.Subtract(new Vec3(radius, radius, radius)), center.Add(new Vec3(radius, radius, radius)));
        return true;
    }
}