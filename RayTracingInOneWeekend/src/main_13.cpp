#include "rtweekend.hpp"

#include "camera_rt.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {
    hittable_list world;
    // 地面，散射材质，通过设置反射率来构造
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.50);
    // 由于这个球体在左球里面，从玻璃到空气的折射率
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    // 往场景中加四个球面，用不同的反射率的材质
    // 地面的大球
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    // 中心的球
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    // 左边的球
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    // 在左边的球的里面(圆心一样，半径不同)
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    // 右边的球
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera_rt cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
}