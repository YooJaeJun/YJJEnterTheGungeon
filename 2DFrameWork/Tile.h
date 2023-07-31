#pragma once

enum class TileState
{
    none,
    floor,
    wall,
    door,
    prop,
    spawner,
    pit,
    max
};

class Tile
{
public:
    int                 F = 0; //타일 비용
    int                 G = 0; //타일 비용
    int                 H = 0; //타일 비용
    Vec2i               idx;    //타일 인덱스
    shared_ptr<Tile>    P;      //나를 갱신시킨 타일
    bool                isFind; //검사한적이 있는가?
    Vector2             Pos; //타일의 중점
    TileState           state;
    int                 roomIdx;
    int                 dir;

    void ClearCost();
    void ClacH(const Vec2i DestIdx);
    void ClacF();
};

using PTile = pair<Tile, int>;

struct ComparePTile
{
    bool operator()(const PTile& a, const PTile& b) const
    {
        return a.second > b.second;
    }
};