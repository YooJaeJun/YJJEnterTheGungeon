#include "framework.h"

void Tile::ClearCost()
{
    F = G = H = INT_MAX;
    P = nullptr;
    isFind = false;
}

void Tile::ClacH(const Vec2i DestIdx)
{
    int tempX = abs(idx.x - DestIdx.x) * 10;
    int tempY = abs(idx.y - DestIdx.y) * 10;

    H = tempX + tempY;
}

void Tile::ClacF()
{
    F = G + H;
}