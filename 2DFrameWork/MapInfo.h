#pragma once
class MapInfo
{
private:
	MapInfo();

public:
	static MapInfo& GetInstance()
	{
		static std::unique_ptr<MapInfo> inst;
		if (!inst)
			inst = std::unique_ptr<MapInfo>(new MapInfo());
		return *inst;
	}

public:
	~MapInfo();
	
	void Init();
	void Update();
	void Render();
	void SetTilemapGUI();

public:
	// Tilemap
	std::shared_ptr<ObTileMap>  tilemap;
	int			imgIdx;
	Vec2i		tileSize;
	Color		tileColor;
	int			tileState;
	Vec2i		mouseIdx;
	Vec2i		pickingIdx;
	bool		useGui;
};

