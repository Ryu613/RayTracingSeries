#pragma once

#include "rtweekend.hpp"

class vec3 {
public:
	// 三个元素的双精度数组，声明三个点xyz
	double e[3];
	// 无参构造器，初始化xyz为0
	vec3() : e{ 0,0,0 } {}
	// 带参构造器: 用三个数初始化xyz
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	// 快捷获取xyz的值
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// 减号操作符重载，即在vec3前面加负号，取-xyz的vec3对象
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	// 用数组下标取值，主要是为了实现const, 注：这里没做越界处理
	double operator[](int i) const { return e[i]; }
	// 用数组下标取引用,可以读写元素
	double& operator[](int i) { return e[i]; }
	// +=操作符重载，每个分量对应加上
	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	// *= 操作符重载，每个分量乘上相同的数
	vec3& operator*=(double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	// /=操作符重载，即*上这个数的倒数
	vec3& operator/=(double t) {
		return *this *= 1 / t;
	}
	// 实际上是取这个向量的模
	double length() const {
		return std::sqrt(length_squared());
	}

	// 模的平方
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	//判断向量是否非常接近0向量
	bool near_zero() const {
		// 一个很小的接近0的数
		auto s = 1e-8;
		// 比较向量的各个分量，取绝对值后，都需要比这个s还小
		return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
	}


	// 获取随机向量（用于模拟哑光材质）
	static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}
	// 限定范围的随机向量
	static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};
// vec3别名
using point3 = vec3;

// 内联函数，定义vec3可以实现的操作
// <<重载 输出 "x y z"格式流
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
// +重载：两个vec3相加，即分量对应相加
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
// -重载: 两个vec3相减， 即分量对应相减
inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
// *重载: 两个vec3相乘， 即分量对应相乘
inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// *重载: 一个数乘以一个vec3,即这个数乘以各个分量
inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

// *重载: 一个vec3乘以一个数，等效于一个数乘以一个vec3
inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

// /重载: 等效于一个数的倒数乘以vec3
inline vec3 operator/(const vec3& v, double t) {
	return (1 / t) * v;
}

// 点乘: 即 两个vec3的分量分别相乘后求和，返回这个数
inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

// 叉乘: 即按叉乘规则生成新的vec3
inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// 取单位向量，即向量除以模
inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}
// 取随机球体内向量
inline vec3 random_unit_vector() {
	// 一直找寻合适的p点
	while (true) {
		// 先在边长为1的立方体范围内生成一个向量p
		auto p = vec3::random(-1, 1);
		// 求出向量p的模的平方(不开根为了计算更简单)
		auto lensq = p.length_squared();
		// 若p模的平方小于等于1，即这个点在球体面或内部
		// 若p这个浮点数过于接近0，由于浮点数误差，会造成模算出来是0，会导致归一化后是正负无穷
		// 为了避免这个问题，限制模的平方需要大于双精度浮点数的最小值
		// 1e-160是经验得来的阈值，是一个极小的接近0的正数，并不是double本身的最小正数极限
		if (1e-160 < lensq && lensq <= 1)
			// 返回归一化的p向量
			return p / sqrt(lensq);
	}
}

// 取与法线同向的随机半球面向量p
inline vec3 random_on_hemisphere(const vec3& normal) {
	// 取出单位球面上随机均匀分布的向量p
	vec3 on_unit_sphere = random_unit_vector();
	// p点点乘法线向量，若大于0，则代表p与法线同向，即p在需要的半球面上
	if (dot(on_unit_sphere, normal) > 0.0)
		return on_unit_sphere;
	// 若不在所需半球面，则反转向量就是了
	else
		return -on_unit_sphere;
}

// 入射光向量v，法线向量n，求反射向量
// 即v+2b=v+2(-(v·n)n)
inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}
//折射函数，给出入射光向量uv, 法线n，η/η'的比值
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	// dot(-uv,n)求cosθ，uv的负号是由于要与法线方向一致
	// 用fmin是因为防止浮点数误差导致的点乘结果略微大于1
	auto cos_theta = std::fmin(dot(-uv, n), 1.0);
	// 折射光线的垂直分量，这里是uv+...是因为n与uv的平行分量方向相反
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	// 折射光线的水平分量，与之前推导公式一致
	vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
	// 水平分量 + 垂直分量即为折射光线的向量
	return r_out_perp + r_out_parallel;
}

// 从圆盘区域选一个随机点
inline vec3 random_in_unit_disk() {
	// 直到选到圆盘里面的点为止
	while (true) {
		// 从[-1,1)中选点(x,y,0)
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		// 若这个点的模小于1(即在单位圆盘里)，则返回这个点
		if (p.length_squared() < 1)
			return p;
	}
}