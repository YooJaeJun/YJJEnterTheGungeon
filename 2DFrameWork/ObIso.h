#pragma once
#define TWODIVROOT3			1.154700538f
#define TWODIVROOT3QUARTER	0.2886751f
#define ROOT3				1.732051015f

class ObIso : public ObTileMap
{
public:
    ObIso();
    ~ObIso();

    bool WorldPosToTileIdx(Vector2 WPos, Vec2i& TileIdx);
    void ResizeTile(Vec2i TileSize);
    void CreateTileCost();
};

