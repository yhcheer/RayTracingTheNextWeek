public class Wrapper{
    Ray scattered;
    Vec3 refracted;
    Vec3 attenuation;
    AABB box;

    public Wrapper() {
        scattered = new Ray();
        attenuation = new Vec3();
        refracted = new Vec3();
        box = new AABB();
    }
}