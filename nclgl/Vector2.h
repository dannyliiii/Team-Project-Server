#pragma once

#include <cmath>
#include <iostream>

class Vector2 {
public:

	Vector2():
		x(0.f), y(0.f) { }

	explicit Vector2(float value):
		x(value), y(value) { }

	Vector2(float x, float y):
		x(x), y(y) { }


	float x;
	float y;


	float const* Data() const {

		return &x;
	}

	float* Data() {

		return &x;
	}


	void ToZero() {

		x = y = 0.f;
	}

	float Length() const {

		return sqrt(x * x + y * y);
	}

	void Normalise() {

		float length = Length();

		if (length != 0.f) {

			length = 1.f / length;

			x *= length;
			y *= length;
		}
	}

	static Vector2 Normalise(const Vector2& a) {

		Vector2 result(a);
		result.Normalise();

		return result;
	}

	static float DistanceSquared(const Vector2& a, const Vector2& b) {

		return pow(a.x - b.x, 2.0f) + pow(a.y - b.y, 2.0f);
	}

	static float Distance(const Vector2& a, const Vector2& b) {

		return sqrtf(pow(a.x - b.x, 2.0f) + pow(a.y - b.y, 2.0f));
	}


	Vector2& operator+=(float value) {

		x += value; y += value;

		return *this;
	}

	Vector2& operator-=(float value) {

		x -= value; y -= value;

		return *this;
	}

	Vector2& operator*=(float value) {

		x *= value; y *= value;

		return *this;
	}

	Vector2& operator/=(float value) {

		x /= value; y /= value;

		return *this;
	}

	Vector2& operator+=(const Vector2& other) {

		x += other.x; y += other.y;

		return *this;
	}

	Vector2& operator-=(const Vector2& other) {

		x -= other.x; y -= other.y;

		return *this;
	}

	Vector2& operator*=(const Vector2& other) {

		x *= other.x; y *= other.y;

		return *this;
	}

	Vector2& operator/=(const Vector2& other) {

		x /= other.x; y /= other.y;

		return *this;
	}

	Vector2 operator-() const {

		return Vector2(-x, -y);
	}
};

inline
std::ostream& operator<<(std::ostream& o, const Vector2& v) {

	return o << "[" << v.x << " " << v.y << "]";
}


inline
bool operator==(float value, const Vector2& a)
{
	return a.x == value && a.y == value;
}

inline
bool operator==(const Vector2& a, float value)
{
	return value == a;
}

inline
bool operator==(const Vector2& a, const Vector2& b) {

	return a.x == b.x && a.y == b.y;
};


inline
bool operator!=(float value, const Vector2& a)
{
	return !(value == a);
}

inline
bool operator!=(const Vector2& a, float value)
{
	return !(a == value);
}

inline
bool operator!=(const Vector2& a, const Vector2& b) {

	return !(a == b);
};


inline
Vector2 operator+(const Vector2& a, float value) {

	Vector2 result(a);
	result += value;

	return result;
}

inline
Vector2 operator-(const Vector2& a, float value) {

	Vector2 result(a);
	result -= value;

	return result;
}

inline
Vector2 operator*(const Vector2& a, float value) {

	Vector2 result(a);
	result *= value;

	return result;
}

inline
Vector2 operator/(const Vector2& a, float value) {

	Vector2 result(a);
	result /= value;

	return result;
}


inline
Vector2 operator+(float value, const Vector2& a) {

	Vector2 result(value);
	result += a;

	return result;
}

inline
Vector2 operator-(float value, const Vector2& a) {

	Vector2 result(value);
	result -= a;

	return result;
}

inline
Vector2 operator*(float value, const Vector2& a) {

	Vector2 result(value);
	result *= a;

	return result;
}

inline
Vector2 operator/(float value, const Vector2& a) {

	Vector2 result(value);
	result /= a;

	return result;
}


inline
Vector2 operator+(const Vector2& a, const Vector2& b) {

	Vector2 result(a);
	result += b;

	return result;
}

inline
Vector2 operator-(const Vector2& a, const Vector2& b) {

	Vector2 result(a);
	result -= b;

	return result;
}

inline
Vector2 operator*(const Vector2& a, const Vector2& b) {

	Vector2 result(a);
	result *= b;

	return result;
}

inline
Vector2 operator/(const Vector2& a, const Vector2& b) {

	Vector2 result(a);
	result /= b;

	return result;
}

