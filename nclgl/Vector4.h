#pragma once

#include "Vector3.h"

#include <cmath>
#include <iostream>

class Vector4 {
public:

	Vector4():
		x(0.f), y(0.f), z(0.f), w(0.f) { }

	explicit Vector4(float value):
		x(value), y(value), z(value), w(value) { }

	Vector4(float x, float y, float z, float w):
		x(x), y(y), z(z), w(w) { }


	float x;
	float y;
	float z;
	float w;

	
	float const* Data() const {
		return &x;
	}

	float* Data() {
		return &x;
	}


	void ToZero() {

		x = y = z = w = 0.0f;
	}

	float Length() const {

		return sqrt(x * x + y * y + z * z + w * w);
	}

	void Normalise() {

		float length = Length();

		if (length != 0.f) {

			length = 1.f / length;

			x *= length;
			y *= length;
			z *= length;
			w *= length;
		}
	}

	static Vector4 Normalise(const Vector4& a) {

		Vector4 result(a);
		result.Normalise();

		return result;
	}

	static float DistanceSquared(const Vector4& a, const Vector4& b) {

		return pow(a.x - b.x, 2.0f) + pow(a.y - b.y, 2.0f) + pow(a.z - b.z, 2.0f) + pow(a.w - b.w, 2.0f);
	}

	static float Distance(const Vector4& a, const Vector4& b) {

		return sqrtf(DistanceSquared(a, b));
	}


	Vector4& operator+=(float value) {

		x += value; y += value; z += value; w += value;

		return *this;
	}

	Vector4& operator-=(float value) {

		x -= value; y -= value; z -= value; w -= value;

		return *this;
	}

	Vector4& operator*=(float value) {

		x *= value; y *= value; z *= value; w *= value;

		return *this;
	}

	Vector4& operator/=(float value) {

		x /= value; y /= value; z /= value; w /= value;

		return *this;
	}

	Vector4& operator+=(const Vector4& other) {

		x += other.x; y += other.y; z += other.z; w += other.w;

		return *this;
	}

	Vector4& operator-=(const Vector4& other) {

		x -= other.x; y -= other.y; z -= other.z; w -= other.w;

		return *this;
	}

	Vector4& operator*=(const Vector4& other) {

		x *= other.x; y *= other.y; z *= other.z; w *= other.w;

		return *this;
	}

	Vector4& operator/=(const Vector4& other) {

		x /= other.x; y /= other.y; z /= other.z; w /= other.w;

		return *this;
	}

	Vector4 operator-() const {

		return Vector4(-x, -y, -z, -w);
	}


	Vector3 ToVector3() {
		return Vector3(x, y, z);
	}
};


inline
std::ostream& operator<<(std::ostream& o, const Vector4& v) {

	return o << "[" << v.x << " " << v.y << " " << v.z << " " << v.w << "]";
}


inline
bool operator==(float value, const Vector4& a)
{
	return a.x == value && a.y == value && a.z == value && a.w == value;
}

inline
bool operator==(const Vector4& a, float value)
{
	return value == a;
}

inline
bool operator==(const Vector4& a, const Vector4& b) {

	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
};


inline
bool operator!=(float value, const Vector4& a)
{
	return !(value == a);
}

inline
bool operator!=(const Vector4& a, float value)
{
	return !(a == value);
}

inline
bool operator!=(const Vector4& a, const Vector4& b) {

	return !(a == b);
};


inline
Vector4 operator+(const Vector4& a, float value) {

	Vector4 result(a);
	result += value;

	return result;
}

inline
Vector4 operator-(const Vector4& a, float value) {

	Vector4 result(a);
	result -= value;

	return result;
}

inline
Vector4 operator*(const Vector4& a, float value) {

	Vector4 result(a);
	result *= value;

	return result;
}

inline
Vector4 operator/(const Vector4& a, float value) {

	Vector4 result(a);
	result /= value;

	return result;
}


inline
Vector4 operator+(float value, const Vector4& a) {

	Vector4 result(value);
	result += a;

	return result;
}

inline
Vector4 operator-(float value, const Vector4& a) {

	Vector4 result(value);
	result -= a;

	return result;
}

inline
Vector4 operator*(float value, const Vector4& a) {

	Vector4 result(value);
	result *= a;

	return result;
}

inline
Vector4 operator/(float value, const Vector4& a) {

	Vector4 result(value);
	result /= a;

	return result;
}


inline
Vector4 operator+(const Vector4& a, const Vector4& b) {

	Vector4 result(a);
	result += b;

	return result;
}

inline
Vector4 operator-(const Vector4& a, const Vector4& b) {

	Vector4 result(a);
	result -= b;

	return result;
}

inline
Vector4 operator*(const Vector4& a, const Vector4& b) {

	Vector4 result(a);
	result *= b;

	return result;
}

inline
Vector4 operator/(const Vector4& a, const Vector4& b) {

	Vector4 result(a);
	result /= b;

	return result;
}
