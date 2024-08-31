#pragma once

#include "vec3.hpp"

#include <iostream>

// 定义vec3别名color
using color = vec3;

// 把[0,1]的RGB值转为[0,255]，并写入输出流里
void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	int rbyte = int(255.999 * r);
	int gbyte = int(255.999 * g);
	int bbyte = int(255.999 * b);

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}