//
// Created by fengxuegt on 2026/7/13.
//

#ifndef GRAPHIC_VECTOR_H
#define GRAPHIC_VECTOR_H
#include <iostream>
#include <assert.h>
namespace math {
    template <class T>
    class Vector3;

    template <class T>
    class Vector4;

    template <class  T>
    class Vector2 {
        public:
            // 构造函数
            Vector2() : x(0), y(0) {}
            Vector2(T x, T y) :x(x), y(y) {}
            Vector2(const Vector2<T> &v) :x(v.x), y(v.y) {}
            Vector2(const Vector3<T> &v) : x(v.x), y(v.y) {}
            Vector2(const Vector4<T> &v) : x(v.x), y(v.y) {}

            // 取得x或者y元素
            const T operator[](int index) const{
                if (index < 0 || index > 2) {
                    assert(false);
                }
                if (index == 0) {
                    return x;
                }
                return y;
            }

            // 向某个元素赋值
            T& operator[](int index) {
                if (index < 0 || index > 2) {
                    assert(false);
                }
                if (index == 0) {
                    return x;
                }
                return y;
            }

            // 等号运算符重载
            Vector2<T> & operator=(const Vector3<T> &v) {
                x = v.x;
                y = v.y;
                return *this;
            }
            Vector2<T> & operator=(const Vector4<T> &v) {
                x = v.x;
                y = v.y;
                return *this;
            }

            // 加法运算符重载 , 添加const代表不能修改自身，这函数应该返回一个新的变量，而不是修改自身
            Vector2<T> operator+(const Vector2<T> &v) const {
                return Vector2<T>(x + v.x, y + v.y);
            }

            // 加法并赋值
            Vector2<T> & operator+=(const Vector2<T> &v) {
                x += v.x;
                y += v.y;
                return *this;
            }

            // 乘法运算符重载
            Vector2<T> operator*(T s) const{
                return Vector2<T>(x * s, y * s);
            }

            Vector2<T> & operator*=(T s) {
                x *= s;
                y *= s;
                return *this;
            }

            // 除法
            Vector2<T> operator/(T s) const {
                assert(s != 0);
                return Vector2<T>(x / s, y / s);
            }
            Vector2<T> & operator/=(T s) {
                assert(s != 0);
                x /= s;
                y /= s;
                return *this;
            }

            Vector2<T> operator-() const {
                return Vector2<T>(-x, -y);
            }
    		void print() {
            	std::cout << "Vector2 is:" << std::endl;
            	std::cout << "x = " << x << ", y = " << y << std::endl;
            	std::cout << std::endl;
            }
        public:
            T x, y;
        };


	template<typename T>
	class Vector3 {
	public:
		Vector3() { x = y = z = 0; }
		Vector3(T x, T y, T z) :x(x), y(y), z(z) {}
		Vector3(const Vector3<T>&v):x(v.x), y(v.y), z(v.z) {}
		Vector3(const Vector4<T>&v):x(v.x), y(v.y), z(v.z) {}

		T operator[](int i) const {
			assert(i >= 0 && i <= 2);

			if (i == 0) return x;
			if (i == 1) return y;

			return z;
		}

		T& operator[](int i) {
			assert(i >= 0 && i <= 2);

			if (i == 0) return x;
			if (i == 1) return y;

			return z;
		}

		Vector3<T> operator=(const Vector2<T>& v) {
			x = v.x; y = v.y;
			return *this;
		}

		Vector3<T> operator=(const Vector4<T>& v) {
			x = v.x; y = v.y; z = v.z;
			return *this;
		}

		Vector3<T> operator+(const Vector3<T>& v) const {
			return Vector3<T>(x + v.x, y + v.y, z + v.z);
		}

		Vector3<T> operator+=(const Vector3<T>& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Vector3<T> operator-(const Vector3<T>& v) const {
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		Vector3<T> operator-=(const Vector3<T>& v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}

		Vector3<T> operator*(T s) const {
			return Vector3(x * s, y * s, z * s);
		}

		Vector3<T> operator*=(T s) {
			x *= s; y *= s; z *= s;
			return *this;
		}

		Vector3<T> operator/(T f) const {
			assert(f != 0);
			float inv = 1.0 / f;
			return Vector3(x * inv, y * inv, z * inv);
		}

		Vector3<T> operator/=(T f) {
			assert(f != 0);
			float inv = 1.0 / f;
			x *= inv; y *= inv; z *= inv;
			return *this;
		}

		Vector3<T>  operator-() const {
			return Vector3<T>(-x, -y, -z);
		}

		void print() {
			std::cout << "Vector3 is:" << std::endl;
			std::cout << "x = " << x << ", y = " << y << ", z = " << z << std::endl;
			std::cout << std::endl;
		}

	public:
		T x, y, z;
	};

	template<typename T>
	class Vector4 {
	public:
		Vector4() { x = y = z = w = 0; }
		Vector4(T x, T y, T z, T w) :x(x), y(y), z(z), w(w) {}
		Vector4(const Vector4<T>&v):x(v.x), y(v.y), z(v.z), w(v.w) {}

		T operator[](int i) const {
			assert(i >= 0 && i <= 3);

			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;

			return w;
		}

		T& operator[](int i) {
			assert(i >= 0 && i <= 3);

			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;

			return w;
		}

		Vector4<T> operator=(const Vector2<T>& v) {
			x = v.x; y = v.y;;
			return *this;
		}

		Vector4<T> operator=(const Vector3<T>& v) {
			x = v.x; y = v.y; z = v.z;
			return *this;
		}

		Vector4<T> operator+(const Vector4<T>& v) const {
			return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		Vector4<T> operator+=(const Vector4<T>& v) {
			x += v.x; y += v.y; z += v.z; w += v.w;
			return *this;
		}

		Vector4<T> operator-(const Vector4<T>& v) const {
			return Vector4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		Vector4<T> operator-=(const Vector4<T>& v) {
			x -= v.x; y -= v.y; z -= v.z; w -= v.w;
			return *this;
		}

		Vector4<T> operator*(T s) const {
			return Vector4(x * s, y * s, z * s, w * s);
		}

		Vector4<T> operator*=(T s) {
			x *= s; y *= s; z *= s; w *= s;
			return *this;
		}

		Vector4<T> operator*=(const Vector3<T>& v) {
			x *= v.x; y *= v.y; z *= v.z;
			return *this;
		}

		Vector4<T> operator/(T f) const {
			assert(f != 0);
			float inv = 1.0 / f;
			return Vector4(x * inv, y * inv, z * inv, w * inv);
		}

		Vector4<T> operator/=(T f) {
			assert(f != 0);
			float inv = 1.0 / f;
			x *= inv; y *= inv; z *= inv; w *= inv;
			return *this;
		}

		Vector4<T> operator-() const {
			return Vector3(-x, -y, -z, -w);
		}

		void print() {
			std::cout << "Vector4 is:" << std::endl;
			std::cout << "x = " << x << ", y = " << y << ", z = " << z << ", w = " << w << std::endl;
			std::cout << std::endl;
		}

	public:
		T x, y, z, w;
	};

	using vec2f = Vector2<float>;
	using vec2i = Vector2<int>;
	using vec3f = Vector3<float>;
	using vec3i = Vector3<int>;
	using vec4f = Vector4<float>;
	using vec4i = Vector4<int>;
}

#endif //GRAPHIC_VECTOR_H