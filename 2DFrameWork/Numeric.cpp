#include "framework.h"

/*
	* @brief use of machine epsilon to compare floating-point values for equality
	* http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
*/

bool almostEqualFloat(const float x, const float y, int ulp)
{
	return fabsf(x - y) <= std::numeric_limits<float>::epsilon() * fabsf(x + y) * static_cast<float>(ulp)
		|| fabsf(x - y) < 1.175494351e-38F; // std::numeric_limits<float>::min();
}

bool almostEqualVector2(const Vector2& v1, const Vector2& v2)
{
	return almostEqualFloat(v1.x, v2.x) && almostEqualFloat(v1.y, v2.y);
}

// Utility 선분 충돌에 사용
int ccw(const Vector2& v1, const Vector2& v2, const Vector2& v3)
{
	float res = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y
		- (v1.y * v2.x + v2.y * v3.x + v3.y * v1.x);
	if (res > 0) return 1;
	if (res < 0) return -1;
	else return 0;
}