#pragma once
class Delaunay
{
public:
	Delaunay() = default;
	Delaunay(const Delaunay&) = delete;

	const std::vector<ObTriangle>& Triangulate(std::vector<ObNode>& vertices);
	bool							ValidChecker(const ObNode& n1, const ObNode& n2);

public:
	vector<ObTriangle>			triangles;
	vector<ObLine>				edges;
	map<ObNode, int>			nodesForIndex;
	map<ObNode, vector<ObNode>>	nodesLinked;
};

