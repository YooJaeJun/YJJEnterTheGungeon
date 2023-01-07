#pragma once
class Map : public Singleton<Map>
{
public:
	Map();
	~Map();
	
	void Init();
	void Release();
	void Update();
	void Render();
	void SetTilemapGUI();

public:
	// Tilemap
	ObTileMap*  tilemap;
	int			imgIdx;
	Int2		tileSize;
	Color		tileColor;
	int			tileState;
	Int2		mouseIdx;
	Int2		pickingIdx;
	bool		useGui;
};

