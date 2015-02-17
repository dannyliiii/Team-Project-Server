#pragma once
/*
Class:Vector3
Implements:
Author:Rich Davison
Description:VERY simple Vector3 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#include <cmath>
#include <iostream>

class Vector3 {
public:

	Vector3():
		x(0.f), y(0.f), z(0.f) { }

	explicit Vector3(float value):
		x(value), y(value), z(value) { }

	Vector3(float x, float y, float z):
		x(x), y(y), z(z) { }


	float x;
	float y;
	float z;


	float const* Data() const {

		return &x;
		}

	float* Data() {

		return &x;
	}

	Vector3 Normal() {
		float length = Length();

		if (length != 0.0f) {
			length = 1.0f / length;
			return Vector3(x * length, y * length, z * length);
		}
		return Vector3(0, 0, 0);
	}

	void		ToZero() {
		x = y = z = 0.0f;
	}

	float Length() const {

		return sqrt(x * x + y * y + z * z);
	}

	void Normalise() {

		float length = Length();

		if (length != 0.f) {

			length = 1.f / length;

			x *= length;
			y *= length;
			z *= length;
	}
	}

	static Vector3 Normalise(const Vector3& a) {

		Vector3 result(a);
		result.Normalise();

		return result;
	}

	static float DistanceSquared(const Vector3& a, const Vector3& b) {

		return pow(a.x - b.x, 2.0f) + pow(a.y - b.y, 2.0f) + pow(a.z - b.z, 2.0f);
	}
	
	static float Distance(const Vector3& a, const Vector3& b) {

		return sqrtf(DistanceSquared(a, b));
	}

	static float Dot(const Vector3 &a, const Vector3 &b) {

		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	static Vector3 Cross(const Vector3 &a, const Vector3 &b) {

		return Vector3(
			a.y * b.z - a.z * b.y, 
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);	
	}


	Vector3& operator+=(float value) {

		x += value; y += value; z += value;

		return *this;
		}

	Vector3& operator-=(float value) {

		x -= value; y -= value; z -= value;

		return *this;
	}

	Vector3& operator*=(float value) {

		x *= value; y *= value; z *= value;

		return *this;
	}

	Vector3& operator/=(float value) {

		x /= value; y /= value; z /= value;

		return *this;
	}

	Vector3& operator+=(const Vector3& other) {

		x += other.x; y += other.y; z += other.z;

		return *this;
	}

	Vector3& operator-=(const Vector3& other) {

		x -= other.x; y -= other.y; z -= other.z;

		return *this;
	}

	Vector3& operator*=(const Vector3& other) {

		x *= other.x; y *= other.y; z *= other.z;

		return *this;
	}

	Vector3& operator/=(const Vector3& other) {

		x /= other.x; y /= other.y; z /= other.z;

		return *this;
	}
	
	Vector3 operator-() const {

		return Vector3(-x, -y, -z);
	}
};


inline
std::ostream& operator<<(std::ostream& o, const Vector3& v) {

	return o << "[" << v.x << " " << v.y << " " << v.z << "]";
	}


inline
bool operator==(float value, const Vector3& a)
{
	return a.x == value && a.y == value && a.z == value;
	}

inline
bool operator==(const Vector3& a, float value)
{
	return value == a;
	}

inline
bool operator==(const Vector3& a, const Vector3& b) {

	return a.x == b.x && a.y == b.y && a.z == b.z;
	};


inline
bool operator!=(float value, const Vector3& a)
{
	return !(value == a);
}

inline
bool operator!=(const Vector3& a, float value)
{
	return !(a == value);
}

inline
bool operator!=(const Vector3& a, const Vector3& b) {

	return !(a == b);
	};


inline
Vector3 operator+(const Vector3& a, float value) {

	Vector3 result(a);
	result += value;

	return result;
}

inline
Vector3 operator-(const Vector3& a, float value) {

	Vector3 result(a);
	result -= value;

	return result;
}

inline
Vector3 operator*(const Vector3& a, float value) {

	Vector3 result(a);
	result *= value;

	return result;
}

inline
Vector3 operator/(const Vector3& a, float value) {

	Vector3 result(a);
	result /= value;

	return result;
}


inline
Vector3 operator+(float value, const Vector3& a) {

	Vector3 result(value);
	result += a;

	return result;
}

inline
Vector3 operator-(float value, const Vector3& a) {

	Vector3 result(value);
	result -= a;

	return result;
}

inline
Vector3 operator*(float value, const Vector3& a) {

	Vector3 result(value);
	result *= a;

	return result;
}

inline
Vector3 operator/(float value, const Vector3& a) {

	Vector3 result(value);
	result /= a;

	return result;
}


inline
Vector3 operator+(const Vector3& a, const Vector3& b) {

	Vector3 result(a);
	result += b;

	return result;
}

inline
Vector3 operator-(const Vector3& a, const Vector3& b) {

	Vector3 result(a);
	result -= b;

	return result;
}

inline
Vector3 operator*(const Vector3& a, const Vector3& b) {

	Vector3 result(a);
	result *= b;

	return result;
}

inline
Vector3 operator/(const Vector3& a, const Vector3& b) {

	Vector3 result(a);
	result /= b;

	return result;
}
