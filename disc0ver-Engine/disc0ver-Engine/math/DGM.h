/*
 * @Description:
 * @Author: 妄想
 * @Email: long452a@163.com
 * @Date: 2020-11-02
 */


#pragma once

#ifndef DGM_H
#define DGM_H

#include <algorithm>
#include <iostream>

namespace disc0ver {
	enum TYPE
	{
		TRANSFORM,
		RGB
	};

	// TODO: 暂时实现 vec3，后面考虑采用 C++ 11 以后的特性进行重写
	template<TYPE T>
	class vec3 {
	public:
		vec3() = default;
		vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

		
		inline float x() { static_assert(T == TRANSFORM, "RGB has no contribute x, please use r"); return e[0]; }
		inline float y() { static_assert(T == TRANSFORM, "RGB has no contribute y, please use g"); return e[1]; }
		inline float z() { static_assert(T == TRANSFORM, "RGB has no contribute z, please use b"); return e[2]; }
		inline float r() { static_assert(T == RGB, "TRANSFORM has no contribute r, please use x"); return e[0]; }
		inline float g() { static_assert(T == RGB, "TRANSFORM has no contribute g, please use y"); return e[1]; }
		inline float b() { static_assert(T == RGB, "TRANSFORM has no contribute b, please use z"); return e[2]; }

		inline const vec3& operator+() const { return *this; }
		inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		inline float operator[](int i) const { return e[i]; }
		inline float& operator[] (int i) { return e[i]; }

		inline vec3& operator+=(const vec3& v) { e[0] += v.e[0]; e[1] += v.e[1]; e[2] += v.e[2]; return *this; }
		inline vec3& operator-=(const vec3& v) { e[0] -= v.e[0]; e[1] -= v.e[1]; e[2] -= v.e[2]; return *this; }
		inline vec3& operator*=(const vec3& v) { e[0] *= v.e[0]; e[1] *= v.e[1]; e[2] *= v.e[2]; return *this; }
		inline vec3& operator/=(const vec3& v) { e[0] /= v.e[0]; e[1] /= v.e[1]; e[2] /= v.e[2]; return *this; }
		inline vec3& operator*=(const float t) { e[0] *= t; e[1] *= t; e[2] *= t; return *this; }
		inline vec3& operator/=(const float t) { float k = 1.0 / t; e[0] *= k; e[1] *= k; e[2] *= k; return *this; }

		inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
		inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
		inline void normalize();

		float e[3];
	};

	using Rotation = vec3<TRANSFORM>;
	using Position = vec3<TRANSFORM>;
	using Scale = vec3<TRANSFORM>;
	using rgb = vec3<RGB>;
	
	template<TYPE T>
	inline void vec3<T>::normalize(){
		float len = this->length();
		for (int i = 0; i < 3; i++) {
			e[i] /= len;
		}
	}

	template<TYPE T> inline std::istream& operator>>(std::istream& is, vec3<T>& t) {
		is >> t.e[0] >> t.e[1] >> t.e[2];
		return is;
	}

	template<TYPE T> inline std::ostream& operator<<(std::ostream& os, const vec3<T>& t) {
		os << t.e[0] << " " << t.e[1] << " " << t.e[2];
		return os;
	}

	template<TYPE T> inline vec3<T> operator+(const vec3<T>& v1, const vec3<T> v2) {
		return vec3<T>(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
	}

	template<TYPE T> inline vec3<T> operator-(const vec3<T>& v1, const vec3<T> v2) {
		return vec3<T>(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
	}

	template<TYPE T> inline vec3<T> operator*(const vec3<T>& v1, const vec3<T> v2) {
		return vec3<T>(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
	}

	template<TYPE T> inline vec3<T> operator/(const vec3<T>& v1, const vec3<T> v2) {
		return vec3<T>(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
	}
	
	template<TYPE T> inline vec3<T> operator*(const vec3<T>& v, float t) {
		return vec3<T>(v.e[0] * t, v.e[1] * t, v.e[2] * t);
	}

	template<TYPE T> inline vec3<T> operator/(const vec3<T>& v, float t) {
		return vec3<T>(v.e[0] / t, v.e[1] / t, v.e[2] / t);
	}

	template<TYPE T> inline float dot(const vec3<T>& v1, const vec3<T>& v2) {
		return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
	}

	inline vec3<TRANSFORM> cross(const vec3<TRANSFORM>& v1, const vec3<TRANSFORM>& v2) {
		return vec3<TRANSFORM>( (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
							   -(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
			                    (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
	}
 }
#endif // !DGM_H
