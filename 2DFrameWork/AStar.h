#pragma once

class AStar
{
private:
	AStar() {}

public:
	static AStar& GetInstance()
	{
		static std::unique_ptr<AStar> inst;
		if (!inst)
			inst = std::unique_ptr<AStar>(new AStar());
		return *inst;
	}

public:
	~AStar() {}

public:
	bool FindPath(std::shared_ptr<ObTileMap> tilemap, Vec2i sour, Vec2i dest, OUT vector<Tile>& way, bool checkDiagnoal = false);
};

