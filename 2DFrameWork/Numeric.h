#include <math.h>
#include <limits>
#include <type_traits>

bool almostEqualFloat(const float x, const float y, int ulp = 2);

bool almostEqualVector2(const Vector2& v1, const Vector2& v2);

// Utility 선분 충돌에 사용
int ccw(const Vector2& v1, const Vector2& v2, const Vector2& v3);