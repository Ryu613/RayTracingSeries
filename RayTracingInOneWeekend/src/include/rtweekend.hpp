#pragma once

#include <cmath>
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

// 通用的头文件

#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"