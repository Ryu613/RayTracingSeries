#include "rtweekend.hpp"

#include "camera_rt.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {
    hittable_list world;
    // 半径
    auto R = std::cos(pi / 4);
    // 哑光材质
    auto material_left = make_shared<lambertian>(color(0, 0, 1));
    auto material_right = make_shared<lambertian>(color(1, 0, 0));
    // 两球，一左一右，相交于画面中点
    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

    camera_rt cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 90;

    cam.render(world);
}