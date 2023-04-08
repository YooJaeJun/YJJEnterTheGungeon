#include <cmath>
#include <climits>
#include <type_traits>


/// <summary>
/// 전역 함수
/// </summary>
bool NearlyEqualFloat(const float x, const float y, int ulp = 2);

bool NearlyEqualVector2(const Vector2& v1, const Vector2& v2);


/// <summary>
/// vector 2d int
/// </summary>
struct Vec2i
{
	int x;
	int y;

	Vec2i() : x(0), y(0) {}
	Vec2i(const int x, const int y)
	{
		this->x = x;
		this->y = y;
	}

	const Vec2i operator+(const Vec2i& dest) const
	{
		return Vec2i(x + dest.x, y + dest.y);
	}
	const Vec2i operator-(const Vec2i& dest) const
	{
		return Vec2i(x - dest.x, y - dest.y);
	}
	const Vec2i operator*(const Vec2i& dest) const
	{
		return Vec2i(x * dest.x, y * dest.y);
	}
	const Vec2i operator/(const Vec2i& dest) const
	{
		return Vec2i(x / dest.x, y / dest.y);
	}

	const Vec2i operator+(const int value) const
	{
		return Vec2i(x + value, y + value);
	}
	const Vec2i operator-(const int value) const
	{
		return Vec2i(x - value, y - value);
	}
	const Vec2i operator*(const int value) const
	{
		return Vec2i(x * value, y * value);
	}
	const Vec2i operator/(const int value) const
	{
		return Vec2i(x / value, y / value);
	}

	bool operator==(const Vec2i dest) const
	{
		return (x == dest.x && y == dest.y);
	}

	bool operator!=(const Vec2i dest) const
	{
		return (x != dest.x || y != dest.y);
	}

	bool operator<(const Vec2i& other) const
	{
		return x < other.x || (!(other.x < x) && y < other.y);
	}
};


/// <summary>
/// vector 2d float
// 추가 이유 - MST 위한 노드인덱스 저장용 + operator>로 priority_queue 정렬용
/// </summary>

struct Vec2f
{
	int		index;
	float	x;
	float	y;

	Vec2f() : index(0), x(0.0f), y(0.0f) {}
	Vec2f(const Vec2f& other) : index(other.index), x(other.x), y(other.y) {}
	Vec2f(const Vector2& other) : index(0), x(other.x), y(other.y) {}
	Vec2f(const int index, const Vector2& other) : index(index), x(other.x), y(other.y) {}
	Vec2f(const int index, const Vec2f& other) : index(index), x(other.x), y(other.y) {}
	Vec2f(const float x, const float y) : index(0), x(x), y(y) {}
	Vec2f(const int index, const float x, const float y) : index(index), x(x), y(y) {}

	inline Vec2f operator+(const Vec2f& other) const
	{
		return Vec2f(x + other.x, y + other.y);
	}
	inline Vec2f operator-(const Vec2f& other) const
	{
		return Vec2f(x - other.x, y - other.y);
	}
	inline Vec2f operator*(const Vec2f& other) const
	{
		return Vec2f(x * other.x, y * other.y);
	}
	inline Vec2f operator/(const Vec2f& other) const
	{
		return Vec2f(x / other.x, y / other.y);
	}

	inline void operator=(const Vec2f& other)
	{
		index = other.index;
		x = other.x;
		y = other.y;
	}

	inline bool operator<(const Vec2f& other) const
	{
		return x < other.x || (!(other.x < x) && y < other.y);
	}
	inline bool operator>(const Vec2f& other) const
	{
		return x > other.x || (!(other.x > x) && y > other.y);
	}

	inline bool operator==(const Vec2f& other) const
	{
		return x == other.x && y == other.y;
	}

	bool NearlyEqualVector2Member(const Vec2f& ohter);

	float DirToRadian(Vec2f Dir);
};