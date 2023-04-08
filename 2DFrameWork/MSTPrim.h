#pragma once

class MSTPrim
{
private:
	MSTPrim() {};

public:
	static MSTPrim& GetInstance()
	{
		static std::unique_ptr<MSTPrim> inst;
		if (!inst)
			inst = std::unique_ptr<MSTPrim>(new MSTPrim());
		return *inst;
	}

public:
	~MSTPrim() {}

public:
	void FindPath(Delaunay& triangulation, const std::vector<ObLine>& linesTriangulated, 
		OUT std::vector<ObLine>& linesMST);

private:
	std::priority_queue<ObLine, vector<ObLine>, greater<ObLine>>	edgePq;
	std::unordered_map<int, bool>		visited;
};

