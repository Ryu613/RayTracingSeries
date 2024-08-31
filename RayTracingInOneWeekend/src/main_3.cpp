#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <iostream>

// 光线返回的颜色，这里暂时设置为黑色
color ray_color(const ray& r) {
    return color(0, 0, 0);
}


int main() {

    // 定义图像的宽高比
    auto aspect_ratio = 16.0 / 9.0;
    // 定义图像的宽度
    int image_width = 400;

    // 计算图像的高度，小于1时取1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // 相机
    // 定义焦距,即相机离视口中心点的距离
    auto focal_length = 1.0;
    // 视口的高度
    auto viewport_height = 2.0;
    // 视口的宽度
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    // 相机的位置，假设在原点
    auto camera_center = point3(0, 0, 0);

    // 定义视口的水平向量u
    auto viewport_u = vec3(viewport_width, 0, 0);
    // 定义视口的垂直向量v（与y轴相反所以取负）
    auto viewport_v = vec3(0, -viewport_height, 0);

    // 视口水平向量u除以图像宽度得到像素点间的水平距离delta u
    auto pixel_delta_u = viewport_u / image_width;
    // 视口垂直向量v除以图像高度得到像素点间的垂直距离delta v
    auto pixel_delta_v = viewport_v / image_height;

    // 计算出视口左上角点Q的位置
    // 相机中心位置 -> 往-z方向移动焦距的距离(视口位置) -> 往视口水平方向的左侧移动一半的向量u的距离 -> 往视口垂直方向的上方移动一般的向量v的距离
    auto viewport_upper_left = camera_center
        - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    // p(0,0)像素点位置：即Q点位置+像素点的水平距离与垂直距离的和的一半
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // 同之前一样，生成图片,也叫渲染（render）

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            // 计算每j行中，视口里的像素点的位置
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            // 计算光线的向量，即，用视口的像素点向量-相机位置的向量可求(没做归一化)
            auto ray_direction = pixel_center - camera_center;
            // 上面求得的光线方向和光线原点就可以生成光线对象
            ray r(camera_center, ray_direction);
            // 把光线传入函数，得到光线应该返回的颜色，这里暂时预设为黑色
            color pixel_color = ray_color(r);
            // 从rgb值的[0,1]区间，转换为[0,255]区间并输出
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}