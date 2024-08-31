#pragma once

// 描述一对起始值的类，比如tmin,tmax
class interval {
public:
    // 最小，最大值，双精度浮点数
    double min, max;
    // 这里最小定义的是正无穷，最大是负无穷
    // 这里的无穷指的是double的最大值
    interval() : min(+infinity), max(-infinity) {}
    // 也可以自己定义最小最大值
    interval(double min, double max) : min(min), max(max) {}
    // 指最大最小值的差
    double size() const {
        return max - min;
    }
    // 一个值是否在[min,max]间
    bool contains(double x) const {
        return min <= x && x <= max;
    }
    // 一个值是否在(min,max)间
    bool surrounds(double x) const {
        return min < x && x < max;
    }
    // 看某数是否超界，若超界，就取离得最近的边界
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
    // 两个预定义的interval对象
    static const interval empty, universe;
};
// 空代表正无穷到负无穷，不会有任何数在之间
const interval interval::empty = interval(+infinity, -infinity);
// 全局，代表负无穷到正无穷，所有数都在里面
const interval interval::universe = interval(-infinity, +infinity);