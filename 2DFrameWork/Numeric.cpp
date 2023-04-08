#include "framework.h"

// float epsilon compare
// http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon

bool AlmostEqualFloat(const float x, const float y, int ulp)
{
	return fabsf(x - y) <= std::numeric_limits<float>::epsilon() * fabsf(x + y) * static_cast<float>(ulp)
		|| fabsf(x - y) < 1.175494351e-38F; // std::numeric_limits<float>::min();
}

bool AlmostEqualVector2(const Vector2& v1, const Vector2& v2)
{
	return AlmostEqualFloat(v1.x, v2.x) && AlmostEqualFloat(v1.y, v2.y);
}