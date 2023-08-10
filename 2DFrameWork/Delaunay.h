#pragma once
class Delaunay
{
public:
	Delaunay() = default;
	Delaunay(const Delaunay&) = delete;

	const std::vector<ObTriangle>& Triangulate(const std::vector<Vec2f>& vertices);
	bool							ValidChecker(const Vec2f& n1, const Vec2f& n2);

public:
	vector<ObTriangle>			triangles;
	vector<ObLine>				edges;
	map<Vec2f, int>				nodesForIndex;
	map<Vec2f, vector<Vec2f>>	nodesLinked;
};
