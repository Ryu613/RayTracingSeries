#pragma once

#include "rtweekend.hpp"

// 类前置声明，避免类循环依赖
class material;

// 用于记录光线与物体碰撞的信息
class hit_record {
public:
    // 记录碰撞点
    point3 p;
    // 碰撞点的法线向量
    vec3 normal;
    shared_ptr<material> mat;
    // 光线的t(r = Q + td中的t),即何时碰撞到的
    double t;
    // 是否是正面(正面就是说光线是从物体外面碰到的)
    bool front_face;
    // 设置面的法线，若是正面，法线朝向物体外，否则，法线朝向物体内
    // 参数outward_normal假设已做归一化，初始法线都是朝外的
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // 光线方向向量点乘物体的法线，若小于0则说明两者方向相反，即是物体外部的光线，即front_face为true
        // 反之则是物体内的光线，即front_face为false
        front_face = dot(r.direction(), outward_normal) < 0;
        // 根据是否正面来取向外的法线还是向内的法线
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// 抽象类：定义可与光线发生碰撞的物体
class hittable {
public:
    // 虚析构函数，不写就默认，否则由派生类实现
    virtual ~hittable() = default;
    // 碰撞检测函数，参数依次为:光线对象ray, tmin, tmax, 碰撞的信息(用来返回)
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};