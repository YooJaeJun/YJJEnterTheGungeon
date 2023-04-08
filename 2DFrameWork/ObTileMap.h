#pragma once

class ObTileMap : public GameObject
{
public:
    enum { tileImageCount = 4 };    // MapInfo에도 있음

public:
    ObTileMap();

    virtual         ~ObTileMap();
    virtual void    CreateTileCost();
    virtual void    ResizeTile(Vec2i TileSize);

    void            RenderGui(Vec2i& GuiPickingIdx, int& ImgIdx);
    void            Render() override;
    void            SetTile(Vec2i TileIdx, Vec2i FrameIdx, int ImgIdx = 0,
                        int TileState = (int)TileState::none, Color color = Color(0.5f, 0.5f, 0.5f, 0.5f), 
                        int roomIndex = -1, DirState dir = DirState::dirNone);

    void            Save();
    virtual void    Load();

    bool            WorldPosToTileIdx(Vector2 Wpos, Vec2i& TileIdx);
    Vector2         TileIdxToWorldPos(Vec2i TileIdx);
    TileState       GetTileState(Vec2i TileIdx);
    void            SetTileState(Vec2i TileIdx, TileState tileState);
    Vec2i           GetTileSize() { return tileSize; };
    Vector2         GetTilePosition(Vec2i TileIdx);
    int             GetTileRoomIndex(Vec2i TileIdx);
    void            SetTileRoomIndex(Vec2i TileIdx, const int tileRoomIndex);
    DirState        GetTileDir(Vec2i TileIdx);
    void            SetTileDir(Vec2i TileIdx, const DirState dir);

    bool            isOnWall(const Vec2i on);
    bool            isInTileState(const Vector2 wpos, const TileState tileState);
    bool            isFootOnWall(const shared_ptr<ObRect> colTile);
    bool            isBodyOnPit(const shared_ptr<ObRect> colTile);

protected:
    std::shared_ptr<VertexTile[]>               vertices;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        vertexBuffer;
    Vec2i                    tileSize;       //10x10 , 4x4

public:
    vector<vector<Tile>>    Tiles;

public:
    shared_ptr<ObImage>     tileImages[tileImageCount];
    string                  file;
};