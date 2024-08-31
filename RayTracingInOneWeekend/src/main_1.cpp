#include <iostream>

int main() {
	int image_width = 256;
	int image_height = 256;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = 0; j < image_height; j++) {
		// 一个输出行数进度的打印
		std::clog << "\r行数还剩余: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			// 实际rgb值，在系统中是[0,1]之间来表示的
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.0;
			// 写入到ppm里面的值[0,255],int强转是舍弃小数位的，保证包含255且不含256，所以用255.999来乘
			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.999 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}

	std::clog << "\r完成                \n";
}