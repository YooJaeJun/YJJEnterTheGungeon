#pragma once

class ObTileMap : public GameObject
{
public:
    enum { tileImageCount = 4 };    // MapInfo에도 있음

public:
    ObTileMap();

    ~ObTileMap() override = default;
    virtual void    CreateTileCost();
    virtual void    ResizeTile(Vec2i TileSize);

    void            RenderGui(Vec2i& GuiPickingIdx, int& ImgIdx) const;
    void            Render() override;
    void SetTile(
        const Vec2i TileIdx, 
        const Vec2i FrameIdx, 
        const int ImgIdx = 0,
        const int TileState = static_cast<int>(TileState::none), 
        const Color color = Color(0.5f, 0.5f, 0.5f, 0.5f), 
        const int roomIndex = -1, DirState dir = DirState::dirNone) const;

    void            Save() const;
    virtual void    Load();

    virtual bool            WorldPosToTileIdx(Vector2 Wpos, Vec2i& TileIdx);
    [[nodiscard]] Vector2         TileIdxToWorldPos(const Vec2i TileIdx) const;
    TileState       GetTileState(const Vec2i TileIdx) const;
    void            SetTileState(const Vec2i TileIdx, TileState tileState) const;
    [[nodiscard]] Vec2i     GetTileSize() const { return tileSize; };
    Vector2         GetTilePosition(const Vec2i TileIdx) const;
    int             GetTileRoomIndex(const Vec2i TileIdx) const;
    void            SetTileRoomIndex(const Vec2i TileIdx, const int tileRoomIndex) const;
    DirState        GetTileDir(const Vec2i TileIdx) const;
    void            SetTileDir(const Vec2i TileIdx, const DirState dir) const;

    bool            isOnWall(const Vec2i on) const;
    bool            isInTileState(const Vector2 wpos, const TileState tileState);
    bool            isFootOnWall(const shared_ptr<ObRect> colTile);
    bool            isBodyOnPit(const shared_ptr<ObRect> colTile);

protected:
    std::shared_ptr<VertexTile[]>               vertices;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        vertexBuffer;
    Vec2i                    tileSize;       //10x10 , 4x4

public:
    vector<vector<Tile>>    Tiles{};

public:
    shared_ptr<ObImage>     tileImages[tileImageCount];
    string                  file;
};