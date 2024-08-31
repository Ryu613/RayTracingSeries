#pragma once

#include <cmath>
#include <iostream>

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