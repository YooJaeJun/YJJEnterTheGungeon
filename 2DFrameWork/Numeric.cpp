#include "framework.h"


/// <summary>
/// 전역 함수
/// </summary>
/// 
// float epsilon compare
// http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon

bool NearlyEqualFloat(const float x, const float y, int ulp)
{
	return fabsf(x - y) <= std::numeric_limits<float>::epsilon() * fabsf(x + y) * static_cast<float>(ulp)
		|| fabsf(x - y) < 1.175494351e-38F; // std::numeric_limits<float>::min();
}

bool NearlyEqualVector2(const Vector2& v1, const Vector2& v2)
{
	return NearlyEqualFloat(v1.x, v2.x) && NearlyEqualFloat(v1.y, v2.y);
}


/// <summary>
/// 멤버 함수
/// </summary>
bool Vec2f::NearlyEqualVector2Member(const Vec2f& ohter)
{
    return NearlyEqualFloat(x, ohter.x) && NearlyEqualFloat(y, ohter.y);
}

float Vec2f::DirToRadian(Vec2f Dir)
{
    //만약 단위벡터가 아니라면
    Vector2 vDir = Vector2(Dir.x, Dir.y);
    vDir.Normalize();
    return atan2f(vDir.y, vDir.x);
}