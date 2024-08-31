#include "rtweekend.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

int main() {
    // 场景中所有的物体的列表
    hittable_list world;
    // 添加了2个球体
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // 定义相机
    camera cam;
    // 设置图像的宽高比
    cam.aspect_ratio = 16.0 / 9.0;
    // 图像的宽度(像素数)
    cam.image_width = 400;
    // 设置每个像素点要采样的点的个数，用于抗锯齿
    cam.samples_per_pixel = 100;
    // 渲染该场景
    cam.render(world);

}