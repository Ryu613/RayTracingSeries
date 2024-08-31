#pragma once

#include "rtweekend.hpp"

// hittable的派生类：球面
class sphere : public hittable {
public:
    // 球面构造器:通过球心点坐标，和半径来构造
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}
    // 重写抽象类的碰撞检测函数，与之前的hit_sphere函数大致相同
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        //根据tmin到tmax的区间，判断哪个根符合
        auto root = (h - sqrtd) / a;
        // 如果第一个根不在[tmin,tmax]范围
        if (root <= ray_tmin || ray_tmax <= root) {
            // 在判断一下第二个跟是否在范围内
            root = (h + sqrtd) / a;
            // 若两个根都不在范围内，则判断为光线不与球面相交
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }
        // 走到这里说明至少有1个根(值小的根优先)符合条件，填充碰撞结果
        rec.t = root;
        // 碰撞点,就是Q+td
        rec.p = r.at(rec.t);
        // 法线，即交点 - 圆心坐标，除以半径(向量的模)就是为了归一化
        vec3 outward_normal = (rec.p - center) / radius;
        // 根据光线是从物体外部穿入还是内部穿出，来设置法线方向
        rec.set_face_normal(r, outward_normal);
        return true;
    }

private:
    // 球面的球心坐标
    point3 center;
    // 球面的半径
    double radius;
};