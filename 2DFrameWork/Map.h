#pragma once
class Map
{
private:
	Map();

public:
	static Map& GetInstance()
	{
		static std::unique_ptr<Map> inst;
		if (!inst)
			inst = std::unique_ptr<Map>(new Map());
		return *inst;
	}

public:
	~Map();
	
	void Init();
	void Update();
	void Render();
	void SetTilemapGUI();

public:
	// Tilemap
	std::shared_ptr<ObTileMap>  tilemap;
	int			imgIdx;
	Int2		tileSize;
	Color		tileColor;
	int			tileState;
	Int2		mouseIdx;
	Int2		pickingIdx;
	bool		useGui;
};

