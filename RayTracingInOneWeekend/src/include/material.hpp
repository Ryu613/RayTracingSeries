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

// 金属材质
class metal : public material {
public:
    // 根据反射率和模糊系数来构造金属材质(模糊系数用来模糊表面的粗糙度)
    // 模糊系数最大为1
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
    // 金属材质的光散射实现
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        // 求得反射向量
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        // 反射向量做归一化后，加上任意单位向量乘模糊系数
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        // 构造反射光对象
        scattered = ray(rec.p, reflected);
        // 反射率赋值
        attenuation = albedo;
        // 取反射光的方向和法线的叉乘是否大于0(即同向)
        // 这里的目的是模拟部分散射出来的光由于与法线向量夹角大于90°而被吸收(不视为散射)
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

// 电导体材质
class dielectric : public material {
public:
    // 用折射系数定义电导体
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    // 散射时计算逻辑
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        // 透明，即rgb都不衰减
        attenuation = color(1.0, 1.0, 1.0);
        // 正面射入光线的话，这个系数就是倒数，从物体里面出来的话，折射系数就是这个数
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;
        // 把入射光做归一化
        vec3 unit_direction = unit_vector(r_in.direction());
        // cosθ,用入射光(反向)与法线向量球叉乘，使用fmin是为了处理浮点误差
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        // 利用刚刚求得的cosθ求出sinθ，这是为了判定η*sinθ是否大于1，若大于1要做全反射处理
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        // 是否能够折射，是则不能折射，否则折射
        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;
        // 处理全反射情况
        // 现在全反射有两种，一种是刚刚提到的全内反射，
        // 还有就是模拟光线并不总是完全反射或完全折射的，而是按照一定概率分布来决定是否反射或折射
        // schlick近似是接于0,1之间的数，大于一个0,1之间的随机数是为了模拟反射率越高，反射概率越高
        // 也就是反射概率等于schlick近似值
        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            // 全反射时，向量是按照反射来算的
            direction = reflect(unit_direction, rec.normal);
        else
        // 不是全反射时，调用折射函数，得出折射光的向量
            direction = refract(unit_direction, rec.normal, ri);

        // 利用散射后的向量和交点p生成散射光线对象
        scattered = ray(rec.p, direction);
        return true;
    }

private:
    // 这个折射率指真空或空气中的折射率，或者材料在包围该材料的介质的折射率之比
    double refraction_index;

    // schlick近似法，用于简化菲涅尔方程计算，
    // cosine就是入射光和法线夹角的余弦，refraction_index就是(相对)折射系数
    static double reflectance(double cosine, double refraction_index) {
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};