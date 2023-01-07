#include "stdafx.h"

namespace Gungeon
{
    Room::Room()
    {
        Init();
    }

    void Room::Init()
    {
        col = new ObRect();

        for (auto& elem : enemySpawner)
        {
            elem = new ObCircle;
            elem->scale = Vector2(20.0f, 20.0f);
            elem->color = Color(1.0f, 0.5f, 0.5f);
            elem->isVisible = false;
        }

        treasureSpawner = new ObCircle;
        treasureSpawner->scale = Vector2(20.0f, 20.0f);
        treasureSpawner->color = Color(0.5f, 0.5f, 1.0f);
        treasureSpawner->isVisible = false;

        for (auto& elem : gateSpawner)
        {
            elem = new ObCircle;
            elem->scale = Vector2(20.0f, 20.0f);
            elem->color = Color(0.5f, 1.0f, 0.5f);
            elem->isVisible = false;
        }
    }

    void Room::Release()
    {
        Character::Release();
    }

    void Room::Update()
    {
        Character::Update();
        for (auto& elem : enemySpawner) elem->Update();
        treasureSpawner->Update();
        for (auto& elem : gateSpawner) elem->Update();
    }

    void Room::LateUpdate()
    {
    }

    void Room::Render()
    {
        Character::Render();
        for (auto& elem : enemySpawner) elem->Render();
        treasureSpawner->Render();
        for (auto& elem : gateSpawner) elem->Render();
    }

    Int2 Room::TileLB() const
    {
        ObRect* r = dynamic_cast<ObRect*>(col);
        Int2 on;
        MAP->tilemap->WorldPosToTileIdx(r->lb(), on);
        return on;
    }

    Int2 Room::TileLT() const
    {
        ObRect* r = dynamic_cast<ObRect*>(col);
        Int2 on;
        MAP->tilemap->WorldPosToTileIdx(r->lt(), on);
        return on;
    }

    Int2 Room::TileRB() const
    {
        ObRect* r = dynamic_cast<ObRect*>(col);
        Int2 on;
        MAP->tilemap->WorldPosToTileIdx(r->rb(), on);
        return on;
    }

    Int2 Room::TileRT() const
    {
        ObRect* r = dynamic_cast<ObRect*>(col);
        Int2 on;
        MAP->tilemap->WorldPosToTileIdx(r->rt(), on);
        return on;
    }

    int Room::TileWidth() const
    {
        return TileRB().x - TileLB().x;
    }

    int Room::TileHeight() const
    {
        return TileLT().y - TileLB().y;
    }
}