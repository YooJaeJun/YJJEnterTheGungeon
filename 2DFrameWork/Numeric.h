#include <math.h>
#include <limits>
#include <type_traits>

bool AlmostEqualFloat(const float x, const float y, int ulp = 2);

bool AlmostEqualVector2(const Vector2& v1, const Vector2& v2);