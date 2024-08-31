#pragma once

#include "vec3.hpp"

class ray {
public:
	ray() {}
    // 构造器： 输入光的原点和方向，构造ray对象
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
    // 获取光线原点
    const point3& origin() const { return orig; }
    // 获取光线的传播方向
    const vec3& direction() const { return dir; }
    // 获取t时间后，光到达的位置的点坐标
    point3 at(double t) const {
        return orig + t * dir;
    }

private:
    // 光线原点
    point3 orig;
    // 光线传播方向
    vec3 dir;
};