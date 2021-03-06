public class MovingSphere extends  Sphere{

    Vec3 center0, center1;
    float radius;
    Material mat;
    float time0, time1;

    public MovingSphere() {
    }

    public MovingSphere(Vec3 center0, Vec3 center1, float time0, float time1, float radius, Material mat) {
        this.center0 = center0;
        this.center1 = center1;
        this.radius = radius;
        this.mat = mat;
        this.time0 = time0;
        this.time1 = time1;
    }

    @Override
    public boolean hit(Ray r, float t_min, float t_max, HitRecord rec) {
        Vec3 oc = r.origin().Subtract(center(r.time));  //球心都修改成和时间相关的位置
        float a = r.direction().dot(r.direction());
        float b = oc.dot(r.direction());
        float c = oc.dot(oc) - radius*radius;
        float discriminant = b*b - a*c;
        if(discriminant > 0)
        {
            //优先选取符合范围的根较小的撞击点，若没有再选取另一个根
            float discFactor = (float)Math.sqrt(discriminant);
            float temp = (-b - discFactor) / (2.0f*a);
            if(temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p.Subtract(center(r.time))).Scale(1.0f/radius);
                rec.matPtr = mat;
                return true;
            }
            temp = (-b + discFactor) / (2.0f*a);
            if(temp < t_max && temp > t_min)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p.Subtract(center(r.time))).Scale(1.0f/radius);
                rec.matPtr = mat;
                return true;
            }
        }
        return false;
    }

    /**
     *运动球体形成一个包围盒
     * @param box0 t0时刻的球
     * @param box1 t1时刻的球
     * @return 返回整个大的包围盒
     */
    public AABB surrounding_box(AABB box0, AABB box1){
        Vec3 small = new Vec3( Math.min(box0.min.x(), box1.min.x()), Math.min(box0.min.y(), box1.min.y()), Math.min(box0.min.z(), box1.min.z()));
        Vec3 big = new Vec3( Math.max(box0.max.x(), box1.max.x()), Math.max(box0.max.y(), box1.max.y()), Math.max(box0.max.z(), box1.max.z()));
        return new AABB(small, big);
    }

    private Vec3 center(float t){
        return center0.Add( center1.Subtract(center0).Scale((t-time0)/(time1-time0)) );
    }


}
