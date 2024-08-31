#pragma once

#include "rtweekend.hpp"

// 定义vec3别名color
using color = vec3;

// 把[0,1]的RGB值转为[0,255]，并写入输出流里
void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// 把r,g,b值控制在[0,1]间
	// 从255改成了256，因为已经用clamp控制了数的范围
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}