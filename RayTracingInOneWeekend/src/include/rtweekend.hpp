#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

// 为了少写几个std::
using std::make_shared;
using std::shared_ptr;

// 常数定义

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// 弧度转角度

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// 获取随机双精度浮点数
inline double random_double() {
    // 返回[0,1)之间的随机数
    return std::rand() / (RAND_MAX + 1.0);
}

// 返回[min, max)内的随机数
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// 通用的头文件

#include "vec3.hpp"
#include "ray.hpp"
#include "interval.hpp"
#include "color.hpp"