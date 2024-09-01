#pragma once

#include "rtweekend.hpp"

#include "hittable.hpp"

class camera_rt {
public:
    double aspect_ratio = 1.0; //图像宽高比
    int image_width = 100; // 图像的宽(像素个数)
    int samples_per_pixel = 10; // 每个像素点的采样点的个数
    int max_depth = 10; // 最大反射光线的次数

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                // 设置像素初始颜色为黑色
                color pixel_color(0, 0, 0);
                // 根据每个像素点需要的采样点数，做循环，为每个采样点生成一个光线，做颜色采样
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    // 从j行i列中取出采样的光线
                    ray r = get_ray(i, j);
                    // 把采样的光线的色彩转换后，累加到当前像素的色彩
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int    image_height;   // 图像的高(类内部通过图像宽计算的，所以私有)
    double pixel_samples_scale; // 对于像素点的所有采样点的和的颜色缩放因子(其实就是采样点数的倒数，用来求平均)
    point3 center;         // 相机的中心点
    point3 pixel00_loc;    // 视口里第一个像素点的位置
    vec3   pixel_delta_u;  // 视口水平方向相邻像素点的间隔距离
    vec3   pixel_delta_v;  // 视口垂直方向相邻像素点的间隔距离

    // 初始化操作
    void initialize() {
        // 计算图像的高
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        // 相机中心
        center = point3(0, 0, 0);

        // 定义焦距,即相机离视口中心点的距离
        auto focal_length = 1.0;
        // 视口的高度
        auto viewport_height = 2.0;
        // 视口的宽度
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // 定义视口的水平向量u
        auto viewport_u = vec3(viewport_width, 0, 0);
        // 定义视口的垂直向量v（与y轴相反所以取负）
        auto viewport_v = vec3(0, -viewport_height, 0);

        // 视口水平向量u除以图像宽度得到像素点间的水平距离delta u
        pixel_delta_u = viewport_u / image_width;
        // 视口垂直向量v除以图像高度得到像素点间的垂直距离delta v
        pixel_delta_v = viewport_v / image_height;

        // 计算出视口左上角点Q的位置
        // 相机中心位置 -> 往-z方向移动焦距的距离(视口位置) -> 往视口水平方向的左侧移动一半的向量u的距离 -> 往视口垂直方向的上方移动一般的向量v的距离
        auto viewport_upper_left = center
            - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        // p(0,0)像素点位置：即Q点位置+像素点的水平距离与垂直距离的和的一半
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    // 根据行列里的第几个像素点，取出采样点的光线
    ray get_ray(int i, int j) const {

        // 取出基于像素点的偏移量
        auto offset = sample_square();
        // 采样点，基于第一个像素点，可以得出第(j,i)个像素点的采样点
        // 使用相邻像素点距离乘(i或j + 随机偏移x或y)
        auto pixel_sample = pixel00_loc
            + ((i + offset.x()) * pixel_delta_u)
            + ((j + offset.y()) * pixel_delta_v);
        // 光线的原点，就是相机中心
        auto ray_origin = center;
        // 光线的方向，就是这个采样点的向量 - 光线的原点
        auto ray_direction = pixel_sample - ray_origin;
        // 构造出一个光线ray对象
        return ray(ray_origin, ray_direction);
    }

    // 取像素点左右上下正负0.5的范围内的点
    vec3 sample_square() const {
        // x,y的值在[-0.5,0.5)区间（不包括0.5）
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    // 获得光线的颜色结果
    color ray_color(const ray& r, int depth, const hittable& world) const {
        // 若超过了光线反射递归次数，则不再收集结果，直接返回黑色
        if (depth <= 0)
            return color(0, 0, 0);
        hit_record rec;
        // 若场景中有物体与光线碰撞
        if (world.hit(r, interval(0.001, infinity), rec)) {
            //// 取反射所在半球面上随机方向
            //vec3 direction = random_on_hemisphere(rec.normal);
            // 用lambert分布来取反射方向
            vec3 direction = rec.normal + random_unit_vector();
            // 递归计算这个反射后的光线的色彩，直到没有反射物，然后把这个颜色取50%
            return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
        }
        // 把光线的方向归一化
        vec3 unit_direction = unit_vector(r.direction());
        // 把归一化的光线的y值(区间在(-1,1))缩放到[0,1]之间，命名为a
        // + 1.0是把归一化的y从[-1,1]平移到了[0,2]
        // 0.5 * 是把范围进一步缩放一半，到[0,1]
        // 这个a只是用来作为一个比例参数，决定蓝色和白色各占多少的比例(以0看作0%,以1看作100%来算)
        auto a = 0.5 * (unit_direction.y() + 1.0);
        // 这个a用于在[0,1]之间做插值，实现颜色的过渡效果
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};