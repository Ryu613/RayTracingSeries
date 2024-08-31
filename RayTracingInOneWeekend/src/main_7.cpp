#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <iostream>

// 判断球面是否与光线相交，参数依次为：球心位置，球体半径，光线对象
// 优化了求根公式，把b=-2h做了替换，消除多余系数
double hit_sphere(const point3& center, double radius, const ray& r) {
    // 求出球心到光源的向量
    vec3 oc = center - r.origin();
    // 根据文中推导，求出二项式的a
    // d点乘d即d的模的平方
    auto a = r.direction().length_squared();
    // 把b换为h
    auto h = dot(r.direction(), oc);
    // 同理求c
    // 点乘替换为模平方
    auto c = oc.length_squared() - radius * radius;
    // b用h替代后消除了系数
    auto discriminant = h * h - a * c;
    // 若h方-ac小于0，即无实根，直接返回-1(不相交)
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        // 若有实根，返回最小的那个（只要最小的t）
        return (h - std::sqrt(discriminant)) / a;
    }
}


// 光线返回的颜色，背景颜色根据高度(y)来决定，越高越蓝，越低越白
// 但是若碰到了球体，则根据归一化的法线分量得出颜色
color ray_color(const ray& r) {
    // 判断光线是否与球面相交
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    // 若有相交且t大于0(t<0时在相机后面，舍去)
    if (t > 0.0) {
        // 用交点减去球心得到法线向量，再做归一化
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        // 对归一化的法线的各个分量做了区间缩放到[0,1],作为rgb色彩
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
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
            // 把光线传入函数，得到光线应该返回的颜色
            color pixel_color = ray_color(r);
            // 从rgb值的[0,1]区间，转换为[0,255]区间并输出
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}