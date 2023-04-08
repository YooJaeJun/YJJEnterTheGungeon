#pragma once

class ObTileMap : public GameObject
{
public:
    enum { tileImageCount = 4 };    // MapInfo에도 있음

public:
    ObTileMap();

    virtual         ~ObTileMap();
    virtual void    CreateTileCost();
    virtual void    ResizeTile(Int2 TileSize);

    void            RenderGui(Int2& GuiPickingIdx, int& ImgIdx);
    void            Render() override;
    void            SetTile(Int2 TileIdx, Int2 FrameIdx, int ImgIdx = 0,
                        int TileState = (int)TileState::none, Color color = Color(0.5f, 0.5f, 0.5f, 0.5f), 
                        int roomIndex = -1, DirState dir = DirState::dirNone);

    void            Save();
    virtual void    Load();

    bool            WorldPosToTileIdx(Vector2 Wpos, Int2& TileIdx);
    Vector2         TileIdxToWorldPos(Int2 TileIdx);
    TileState       GetTileState(Int2 TileIdx);
    void            SetTileState(Int2 TileIdx, TileState tileState);
    Int2            GetTileSize() { return tileSize; };
    Vector2         GetTilePosition(Int2 TileIdx);
    int             GetTileRoomIndex(Int2 TileIdx);
    void            SetTileRoomIndex(Int2 TileIdx, const int tileRoomIndex);
    DirState        GetTileDir(Int2 TileIdx);
    void            SetTileDir(Int2 TileIdx, const DirState dir);

    bool            isOnWall(const Int2 on);
    bool            isInTileState(const Vector2 wpos, const TileState tileState);
    bool            isFootOnWall(const shared_ptr<ObRect> colTile);
    bool            isBodyOnPit(const shared_ptr<ObRect> colTile);

protected:
    std::shared_ptr<VertexTile[]>               vertices;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        vertexBuffer;
    Int2                    tileSize;       //10x10 , 4x4

public:
    vector<vector<Tile>>    Tiles;

public:
    shared_ptr<ObImage>     tileImages[tileImageCount];
    string                  file;
};