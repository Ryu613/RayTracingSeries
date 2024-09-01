#pragma once

#include "hittable.hpp"

class material {
public:
    virtual ~material() = default;

    // 判断光线是否反射的虚函数，参数先后是：光线对象，反射相关的数据，光线衰减的量(rgb), 反射出来的光线ray对象
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

// Lambertian材质
class lambertian : public material {
public:
    // 输入反射率来构造此材质
    lambertian(const color& albedo) : albedo(albedo) {}
    
    // 散射函数，参数依次是：光线对象，碰撞相关数据， 衰减值，散射后的光线对象
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        // 生成符合lambertian分布的光线散射向量
        auto scatter_direction = rec.normal + random_unit_vector();
        // 处理当生成的随机单位向量与法线非常接近时出现的散射向量过于接近0的问题
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        // 用这个光线向量生成散射的光线对象
        scattered = ray(rec.p, scatter_direction);
        // 衰减度就是反射率
        attenuation = albedo;
        return true;
    }

private:
    // 光线的反射率
    color albedo;
};