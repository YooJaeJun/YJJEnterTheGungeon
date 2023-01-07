#include "framework.h"

ObIso::ObIso()
{
    SafeRelease(vertexBuffer);
    SafeDeleteArray(vertices);

    tileSize.x = 20;
    tileSize.y = 20;
    file = "map1.txt";
    scale = Vector2(50.0f, 50.0f);
    tileImages[0] = make_shared<ObImage>(L"Iso.png");
    tileImages[0]->maxFrame = Int2(8, 7);
    tileImages[1] = nullptr;
    tileImages[2] = nullptr;
    tileImages[3] = nullptr;

    ResizeTile(tileSize);
}

ObIso::~ObIso()
{
    SafeRelease(vertexBuffer);
    SafeDeleteArray(vertices);
}

bool ObIso::WorldPosToTileIdx(Vector2 WPos, Int2& TileIdx)
{
    WPos -= GetWorldPos();

    Vector2 tileCoord;
    tileCoord.x = WPos.x / scale.x;
    tileCoord.y = WPos.y / scale.y * ROOT3;

    // x
    // (j - i) * 0.5
    // (j - i) * 0.5 - (-(j + i) * 0.5)
    // ((j - i) + (j + i)) * 0.5
    // 2j * 0.5
    // j

    // y
    // -(j + i) * TWODIVROOT3QUARTER * ROOT3
    // -(j + i) * 0.5
    // -((j - i) * 0.5) - -(j + i) * 0.5
    // (-(j - i) * 0.5) - -(j + i) * 0.5
    // ( (-(j - i) + (j + i)) ) * 0.5
    // 2i * 0.5
    // i

    Vector2 temp;
    temp.x = (int)(tileCoord.x - tileCoord.y);
    temp.y = (int)(-tileCoord.x - tileCoord.y);

    TileIdx.x = temp.x;
    TileIdx.y = temp.y;

    if ((temp.x < 0) or (temp.y < 0) or
        (TileIdx.x >= tileSize.x) or (TileIdx.y >= tileSize.y))
    {
        return false;
    }
    return true;
}

void ObIso::ResizeTile(Int2 TileSize)
{
    VertexTile* Vertices = new VertexTile[TileSize.x * TileSize.y * 6];

    // Init
    for (int i = 0; i < TileSize.y; i++)
    {
        for (int j = 0; j < TileSize.x; j++)
        {
            // Å¸ÀÏÁÂÇ¥ ( tileSize.x * yÁÂÇ¥ + xÁÂÇ¥ )
            // ²ÀÁöÁ¡ ÁÂÇ¥ ( tileSize.x * yÁÂÇ¥ + xÁÂÇ¥ ) * 6
            int tileIdx = TileSize.x * i + j;
            Vertices[tileIdx * 6].position.x = -0.5f + (j - i) * 0.5f;
            Vertices[tileIdx * 6].position.y = -TWODIVROOT3 - (j + i) * TWODIVROOT3QUARTER;
            Vertices[tileIdx * 6].uv = Vector2(0.0f, 1.0f);
            //1                             
            Vertices[tileIdx * 6 + 1].position.x = -0.5f + (j - i) * 0.5f;
            Vertices[tileIdx * 6 + 1].position.y = 0.0f - (j + i) * TWODIVROOT3QUARTER;
            Vertices[tileIdx * 6 + 1].uv = Vector2(0.0f, 0.0f);
            //2                             
            Vertices[tileIdx * 6 + 2].position.x = 0.5f + (j - i) * 0.5f;
            Vertices[tileIdx * 6 + 2].position.y = -TWODIVROOT3 - (j + i) * TWODIVROOT3QUARTER;
            Vertices[tileIdx * 6 + 2].uv = Vector2(1.0f, 1.0f);
            //3
            Vertices[tileIdx * 6 + 3].position.x = 0.5f + (j - i) * 0.5f;
            Vertices[tileIdx * 6 + 3].position.y = 0.0f - (j + i) * TWODIVROOT3QUARTER;
            Vertices[tileIdx * 6 + 3].uv = Vector2(1.0f, 0.0f);
            //4
            Vertices[tileIdx * 6 + 4].position.x = 0.5f + (j - i) * 0.5f;
            Vertices[tileIdx * 6 + 4].position.y = -TWODIVROOT3 - (j + i) * TWODIVROOT3QUARTER;
            Vertices[tileIdx * 6 + 4].uv = Vector2(1.0f, 1.0f);
            //5
            Vertices[tileIdx * 6 + 5].position.x = -0.5f + (j - i) * 0.5f;
            Vertices[tileIdx * 6 + 5].position.y = 0.0f - (j + i) * TWODIVROOT3QUARTER;
            Vertices[tileIdx * 6 + 5].uv = Vector2(0.0f, 0.0f);
        }
    }

    // Copy
    if (vertices)
    {
        Int2 Min = Int2(min(TileSize.x, tileSize.x), min(TileSize.y, tileSize.y));
        for (int i = 0; i < Min.y; i++)
        {
            for (int j = 0; j < Min.x; j++)
            {
                int SrcIdx = tileSize.x * i + j;
                int DestIdx = TileSize.x * i + j;
                for (int k = 0; k < 6; k++)
                {
                    Vertices[DestIdx * 6 + k] = vertices[SrcIdx * 6 + k];
                }
            }
        }
    }
    SafeDeleteArray(vertices);
    vertices = Vertices;
    tileSize = TileSize;
    SafeRelease(vertexBuffer);
    // CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(VertexTile) * tileSize.x * tileSize.y * 6;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        Check(hr);
    }
}

void ObIso::CreateTileCost()
{
    for (int i = 0; i < Tiles.size(); i++)
    {
        Tiles[i].clear();
    }
    Tiles.clear();

    Tiles.resize(tileSize.x);
    for (int i = 0; i < Tiles.size(); i++)
    {
        Tiles[i].resize(tileSize.y);
    }

    float half = scale.y * TWODIVROOT3QUARTER;
    Update();
    for (int i = 0; i < tileSize.x; i++)
    {
        for (int j = 0; j < tileSize.y; j++)
        {
            Tiles[i][j].idx = Int2(i, j);
            Tiles[i][j].state = GetTileState(Tiles[i][j].idx);
            Tiles[i][j].Pos.x = -(j - i) * 0.5f * scale.x + GetWorldPos().x;
            Tiles[i][j].Pos.y = -(j + i) * TWODIVROOT3QUARTER * scale.y + GetWorldPos().y - half;
        }
    }
}
