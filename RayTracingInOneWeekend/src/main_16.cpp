#include "rtweekend.hpp"

#include "camera_rt2.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {
    hittable_list world;

    // 生成一个哑光材质，灰白色
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // 在场景里加一个球，非常大的球，作为地面
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    // 这里2层循环是为了随机取球心的坐标，生成各种材质的球
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            // 选取一个随机数，用于材质抽奖
            auto choose_mat = random_double();
            // 给出一个球心的位置，可以看到y固定0.2，xz是用a,b再加随机数的0.9倍得到的
            // 这里为了保证这些随机球心的球体不会重合
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            // 球心不能在这个范围内(留出一些空间生成特殊球体)
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                // 如果随机数小于0.8,生成哑光材质小球，颜色也随机
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    // 半径为0.2
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                // 如果随机数小于0.95其实是[0.8,0.95)),金属材质小球，粗糙度和颜色(深色范围)随机，半径0.2
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                // [0.95,1) 玻璃球，电导体材质，半径0.2
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // 单独加3个球，放在刚刚空出来的位置
    // 三种材质下，半径为1的球
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera_rt2 cam;
    // 视口宽高比，图像宽度，每个像素采样点个数，光线最大迭代次数设置
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    // 相机视场角，位置，朝向设置
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    // 景深参数
    // 失焦角度
    cam.defocus_angle = 0.6;
    // 焦点距离
    cam.focus_dist = 10.0;

    cam.render(world);
}