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
    int                 F = 0; //Ÿ�� ���
    int                 G = 0; //Ÿ�� ���
    int                 H = 0; //Ÿ�� ���
    Vec2i               idx;    //Ÿ�� �ε���
    shared_ptr<Tile>    P;      //���� ���Ž�Ų Ÿ��
    bool                isFind; //�˻������� �ִ°�?
    Vector2             Pos; //Ÿ���� ����
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