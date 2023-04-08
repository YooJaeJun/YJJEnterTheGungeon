#include "stdafx.h"

namespace Gungeon
{
    Room::Room()
    {
        Init();
    }

    void Room::Init()
    {
        col = make_shared<ObRect>();

        for (auto& elem : enemySpawner)
        {
            elem = make_shared<ObCircle>();
            elem->scale = Vector2(20.0f, 20.0f);
            elem->color = Color(1.0f, 0.5f, 0.5f);
            elem->isVisible = false;
        }

        treasureSpawner = make_shared<ObCircle>();
        treasureSpawner->scale = Vector2(20.0f, 20.0f);
        treasureSpawner->color = Color(0.5f, 0.5f, 1.0f);
        treasureSpawner->isVisible = false;

        for (auto& elem : gateSpawner)
        {
            elem = make_shared<ObCircle>();
            elem->scale = Vector2(20.0f, 20.0f);
            elem->color = Color(0.5f, 1.0f, 0.5f);
            elem->isVisible = false;
        }
    }

    void Room::Update()
    {
        Character::Update();
        for (auto& elem : enemySpawner) 
            elem->Update();
        treasureSpawner->Update();
        for (auto& elem : gateSpawner) 
            elem->Update();
    }

    void Room::LateUpdate()
    {
    }

    void Room::Render()
    {
        Character::Render();
        for (auto& elem : enemySpawner) 
            elem->Render();
        treasureSpawner->Render();
        for (auto& elem : gateSpawner) 
            elem->Render();
    }

    Vec2i Room::TileLB() const
    {
        shared_ptr<ObRect> r = dynamic_pointer_cast<ObRect>(col);
        Vec2i on;
        MAPINFO.tilemap->WorldPosToTileIdx(r->lb(), on);
        return on;
    }

    Vec2i Room::TileLT() const
    {
        shared_ptr<ObRect> r = dynamic_pointer_cast<ObRect>(col);
        Vec2i on;
        MAPINFO.tilemap->WorldPosToTileIdx(r->lt(), on);
        return on;
    }

    Vec2i Room::TileRB() const
    {
        shared_ptr<ObRect> r = dynamic_pointer_cast<ObRect>(col);
        Vec2i on;
        MAPINFO.tilemap->WorldPosToTileIdx(r->rb(), on);
        return on;
    }

    Vec2i Room::TileRT() const
    {
        shared_ptr<ObRect> r = dynamic_pointer_cast<ObRect>(col);
        Vec2i on;
        MAPINFO.tilemap->WorldPosToTileIdx(r->rt(), on);
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