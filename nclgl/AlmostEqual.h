#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include <limits>

// From http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
inline
bool AlmostEqual(float a, float b, float maxRelativeDifference = std::numeric_limits<float>::epsilon())
{
	float difference = std::abs(a - b);

	a = std::abs(a);
	b = std::abs(b);

	float largest = a > b ? a : b;

	return difference <= largest * maxRelativeDifference;
}

inline
bool AlmostEqual(const Vector2& v1, const Vector2& v2, float maxRelativeDifference = std::numeric_limits<float>::epsilon())
{
	return
		AlmostEqual(v1.x, v2.x, maxRelativeDifference) &&
		AlmostEqual(v1.y, v2.y, maxRelativeDifference);
}

inline
bool AlmostEqual(const Vector3& v1, const Vector3& v2, float maxRelativeDifference = std::numeric_limits<float>::epsilon())
{
	return
		AlmostEqual(v1.x, v2.x, maxRelativeDifference) &&
		AlmostEqual(v1.y, v2.y, maxRelativeDifference) &&
		AlmostEqual(v1.z, v2.z, maxRelativeDifference);
}

inline
bool AlmostEqual(const Vector4& v1, const Vector4& v2, float maxRelativeDifference = std::numeric_limits<float>::epsilon())
{
	return
		AlmostEqual(v1.x, v2.x, maxRelativeDifference) &&
		AlmostEqual(v1.y, v2.y, maxRelativeDifference) &&
		AlmostEqual(v1.z, v2.z, maxRelativeDifference) &&
		AlmostEqual(v1.w, v2.w, maxRelativeDifference);
}
