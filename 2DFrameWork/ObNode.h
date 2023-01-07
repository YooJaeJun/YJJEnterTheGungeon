#pragma once

// 추가 이유 - MST 위한 노드인덱스 저장용 + operator>로 priority_queue 정렬용
class ObNode
{
public:
	int		index;
	float	x;
	float	y;

public:
	ObNode() : index(0), x(0.0f), y(0.0f) {}
	ObNode(const ObNode& other) : index(other.index), x(other.x), y(other.y) {}
	ObNode(const Vector2& other) : index(0), x(other.x), y(other.y) {}
	ObNode(const int index, const Vector2& other) : index(index), x(other.x), y(other.y) {}
	ObNode(const int index, const ObNode& other) : index(index), x(other.x), y(other.y) {}
	ObNode(const float x, const float y) : index(0), x(x), y(y) {}
	ObNode(const int index, const float x, const float y) : index(index), x(x), y(y) {}

	inline ObNode operator+(const ObNode& other) const
	{
		return ObNode(x + other.x, y + other.y);
	}
	inline ObNode operator-(const ObNode& other) const
	{
		return ObNode(x - other.x, y - other.y);
	}
	inline ObNode operator*(const ObNode& other) const
	{
		return ObNode(x * other.x, y * other.y);
	}
	inline ObNode operator/(const ObNode& other) const
	{
		return ObNode(x / other.x, y / other.y);
	}

	inline void operator=(const ObNode& other)
	{
		index = other.index;
		x = other.x;
		y = other.y;
	}

	inline bool operator<(const ObNode& other) const
	{
		return x < other.x || (!(other.x < x) && y < other.y);
	}
	inline bool operator>(const ObNode& other) const
	{
		return x > other.x || (!(other.x > x) && y > other.y);
	}

	inline bool operator==(const ObNode& other) const
	{
		return x == other.x && y == other.y;
	}

public:
	bool almostEqualNode(const ObNode& ohter);

	float DirToRadian(ObNode Dir);
};