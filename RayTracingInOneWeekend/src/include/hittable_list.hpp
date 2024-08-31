#pragma once

#include "rtweekend.hpp"
#include <vector>

// 存储所有可能与光线发生碰撞的物体的派生类
class hittable_list : public hittable {
public:
    // 存储所有可能与光线有碰撞的派生类列表
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    // 构造器：可通过一个hittable对象创建，并把这个对象加到objects列表里
    hittable_list(shared_ptr<hittable> object) { add(object); }
    // 清楚objects列表所有内容
    void clear() { objects.clear(); }
    // 添加一个hittable对象
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }
    // 检测这些物体是否与光线碰撞，若有，则返回objects列表里的最后一个碰撞信息，并且返回true
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        // 代表objects是否有物体与光线碰撞,目前初始化维false
        bool hit_anything = false;
        // 代表目前最远的，与光线有碰撞的物体的时间t,目前初始化为tmax
        auto closest_so_far = ray_tmax;
        // 遍历所有objects
        for (const auto& object : objects) {
            // 若碰撞了，则把碰撞数据取出来，更新closest_so_far，把hit_anything改为true
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                // 把最近t更新为最小的碰撞t,下次循环就只取得到更小的t，这样来找到最近的t
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};
