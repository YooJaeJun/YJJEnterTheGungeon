#pragma once

struct VertexPC
{
    Vector3 position;
    Color   color;

    VertexPC() { position.z = 0.0f; }

    VertexPC(Vector3 position, Color  color)
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

    VertexPT(Vector3 position, Vector2  uv)
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

    float tileMapIdx;
    float tileState;
    int tileRoomIdx;
    int tileDir;

    VertexTile()
    {
        position = Vector3(0.0f, 0.0f, 0.0f);
        uv = Vector2(0.0f, 0.0f);
        color = Color(0.5f, 0.5f, 0.5f, 0.5f);
        tileMapIdx = 0.0f;
        tileState = 0.0f;
        tileRoomIdx = -1;
        tileDir = -1;
    }

    VertexTile(Vector3 position, Vector2 uv, Color color, float tileMapIdx, float tileState)
        : position(position), uv(uv), color(color), tileMapIdx(tileMapIdx), tileState(tileState), tileRoomIdx(-1), tileDir(-1)
    {
        this->position.z = 0.0f;
    }
};