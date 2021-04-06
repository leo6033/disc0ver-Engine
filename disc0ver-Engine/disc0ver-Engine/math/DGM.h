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
	/*
		 枚举类型
		 transform代表变换 使用x、y、z获取成员
		 rgb代表颜色 使用r、g、b获取成员
	*/
	enum class Vec3Type
	{
		TRANSFORM,
		RGB
	};

	// TODO: 暂时实现 vec3，后面考虑采用 C++ 11 以后的特性进行重写
	template<Vec3Type T>
	class vec3 {
	public:
		vec3() = default;
		vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
		vec3(const glm::vec3& v) { e[0] = v.x, e[1] = v.y, e[2] = v.z; }

		
		// get
		inline float get_x() const { static_assert(T == Vec3Type::TRANSFORM, "Vec3Type::RGB has no contribute x, please use r"); return e[0] ; }
		inline float get_y() const { static_assert(T == Vec3Type::TRANSFORM, "Vec3Type::RGB has no contribute y, please use g"); return e[1]; }
		inline float get_z() const { static_assert(T == Vec3Type::TRANSFORM, "Vec3Type::RGB has no contribute z, please use b"); return e[2]; }
		inline float get_r() const { static_assert(T == Vec3Type::RGB, "Vec3Type::TRANSFORM has no contribute r, please use x"); return e[0]; }
		inline float get_g() const { static_assert(T == Vec3Type::RGB, "Vec3Type::TRANSFORM has no contribute g, please use y"); return e[1]; }
		inline float get_b() const { static_assert(T == Vec3Type::RGB, "Vec3Type::TRANSFORM has no contribute b, please use z"); return e[2]; }

		// set-[
		inline void set_x(float value) { static_assert(T == Vec3Type::TRANSFORM, "Vec3Type::RGB has no contribute x, please use r"); e[0] = value; }
		inline void set_y(float value) { static_assert(T == Vec3Type::TRANSFORM, "Vec3Type::RGB has no contribute y, please use g"); e[1] = value; }
		inline void set_z(float value) { static_assert(T == Vec3Type::TRANSFORM, "Vec3Type::RGB has no contribute z, please use b"); e[2] = value; }
		inline void set_r(float value) { static_assert(T == Vec3Type::RGB, "Vec3Type::TRANSFORM has no contribute r, please use x"); e[0] = value; }
		inline void set_g(float value) { static_assert(T == Vec3Type::RGB, "Vec3Type::TRANSFORM has no contribute g, please use y"); e[1] = value; }
		inline void set_b(float value) { static_assert(T == Vec3Type::RGB, "Vec3Type::TRANSFORM has no contribute b, please use z"); e[2] = value; }

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
		inline vec3& operator=(const glm::vec3& v) { e[0] = v.x; e[1] = v.y; e[2] = v.z; return *this; }
		inline operator glm::vec3() const { return glm::vec3(e[0], e[1], e[2]); }

		inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
		inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
		inline void normalize();
		
		float e[3];
		/*
			设置属性 可以通过.x .y .z 或者 .r .g .b的形式访问/修改成员
		*/
		__declspec(property(get = get_x, put = set_x)) float x;
		__declspec(property(get = get_y, put = set_y)) float y;
		__declspec(property(get = get_z, put = set_z)) float z;
		__declspec(property(get = get_r, put = set_r)) float r;
		__declspec(property(get = get_g, put = set_g)) float g;
		__declspec(property(get = get_b, put = set_b)) float b;
	};

	using Rotation = vec3<Vec3Type::TRANSFORM>;
	using Position = vec3<Vec3Type::TRANSFORM>;
	using Scale = vec3<Vec3Type::TRANSFORM>;
	using Rgb = vec3<Vec3Type::RGB>;
	
	template<Vec3Type T>
	inline void vec3<T>::normalize(){
		float len = this->length();
		for (int i = 0; i < 3; i++) {
			e[i] /= len;
		}
	}

	template<Vec3Type T> inline std::istream& operator>>(std::istream& is, vec3<T>& t) {
		is >> t.e[0] >> t.e[1] >> t.e[2];
		return is;
	}

	template<Vec3Type T> inline std::ostream& operator<<(std::ostream& os, const vec3<T>& t) {
		os << t.e[0] << " " << t.e[1] << " " << t.e[2];
		return os;
	}

	template<Vec3Type T> inline vec3<T> operator+(const vec3<T>& v1, const vec3<T>& v2) {
		return vec3<T>(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
	}

	template<Vec3Type T> inline vec3<T> operator-(const vec3<T>& v1, const vec3<T>& v2) {
		return vec3<T>(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
	}

	template<Vec3Type T> inline vec3<T> operator*(const vec3<T>& v1, const vec3<T>& v2) {
		return vec3<T>(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
	}

	template<Vec3Type T> inline vec3<T> operator/(const vec3<T>& v1, const vec3<T>& v2) {
		return vec3<T>(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
	}
	
	template<Vec3Type T> inline vec3<T> operator*(const vec3<T>& v, float t) {
		return vec3<T>(v.e[0] * t, v.e[1] * t, v.e[2] * t);
	}

	template<Vec3Type T> inline vec3<T> operator/(const vec3<T>& v, float t) {
		return vec3<T>(v.e[0] / t, v.e[1] / t, v.e[2] / t);
	}

	template<Vec3Type T> inline float dot(const vec3<T>& v1, const vec3<T>& v2) {
		return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
	}

	inline vec3<Vec3Type::TRANSFORM> cross(const vec3<Vec3Type::TRANSFORM>& v1, const vec3<Vec3Type::TRANSFORM>& v2) {
		return vec3<Vec3Type::TRANSFORM>( (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
							   -(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
			                    (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
	}
 }
#endif // !DGM_H
