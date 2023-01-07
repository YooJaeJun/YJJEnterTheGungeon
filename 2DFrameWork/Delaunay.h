#pragma once
class Delaunay
{
public:
	vector<ObTriangle>			triangles;
	vector<ObLine>				edges;
	map<ObNode, int>			nodesForIndex;
	map<ObNode, vector<ObNode>>	nodesLinked;

public:
	Delaunay() = default;
	Delaunay(const Delaunay&) = delete;

	const std::vector<ObTriangle>& triangulate(std::vector<ObNode>& vertices);
	bool validChecker(const ObNode& n1, const ObNode& n2);
};

