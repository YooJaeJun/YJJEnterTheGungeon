#pragma once

struct VertexPC
{
    Vector3 position;
    Color   color;

    VertexPC() { position.z = 0.0f; }

    VertexPC(const Vector3& position, const Color& color)
        : position(position), color(color)
    {
        this->position.z = 0.0f;
    }
};


struct VertexPT
{
    Vector3 position;
    //ÅØ½ºÃÄ ÁÂÇ¥ (0~1)
    Vector2 uv;

    VertexPT() { position.z = 0.0f; }

    VertexPT(const Vector3& position, const Vector2& uv)
        : position(position), uv(uv)
    {
        this->position.z = 0.0f;
    }
};

struct VertexTile
{
    Vector3 position;
    Vector2 uv;
    Color color;

    int tileMapIdx;
    int tileState;
    int tileRoomIdx;
    int tileDir;

    VertexTile()
    {
        position = Vector3(0.0f, 0.0f, 0.0f);
        uv = Vector2(0.0f, 0.0f);
        color = Color(0.5f, 0.5f, 0.5f, 0.5f);
        tileMapIdx = 0;
        tileState = 0;
        tileRoomIdx = -1;
        tileDir = -1;
    }

    VertexTile(const Vector3& position, const Vector2& uv, const Color& color, const int tileMapIdx, const int tileState)
        : position(position), uv(uv), color(color), tileMapIdx(tileMapIdx), tileState(tileState), tileRoomIdx(-1), tileDir(-1)
    {
        this->position.z = 0.0f;
    }
};