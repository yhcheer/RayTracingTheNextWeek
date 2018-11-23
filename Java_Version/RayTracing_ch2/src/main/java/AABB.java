public class AABB {

    /** 最小坐标 */
    Vec3 min;
    /** 最大坐标 */
    Vec3 max;

    public AABB() {
    }
    public AABB(Vec3 min, Vec3 max) {
        this.min = min;
        this.max = max;
    }

    Vec3 getMin(){
        return this.min;
    }
    Vec3 getMax(){
        return this.max;
    }

    /**
     * 判断光线和AABB是否有碰撞 必须三个维度都有重叠
     * @param r 光线
     * @param tmin 光线开始时间
     * @param tmax 光线结束时间
     * @return 三个维度都有重叠 返回true
     */
    public boolean hit(Ray r, float tmin, float tmax){
        for(int i = 0; i < 3; i++){ //三个维度
            float invD = 1.0f / r.direction().e[i];
            float t0 = ( min.e[i] - r.direction().e[i] ) * invD;
            float t1 = ( max.e[i] - r.direction().e[i] ) * invD;

            if(invD < 0.0f){    //B<0 光线是反方向传播 即t1 < t0
                float temp = t0;
                t0 = t1;
                t1 = temp;
            }
            tmin = t0>tmin ? t0:tmin;   //下限和t0取大者
            tmax = t1<tmax ? t1:tmax;   //上限和t1取小者
            if(tmax <= tmin){
                return false;
            }
        }
        return true;
    }
}
