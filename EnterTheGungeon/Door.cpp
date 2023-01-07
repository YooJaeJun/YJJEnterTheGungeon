#include "stdafx.h"

namespace Gungeon
{
    Door::Door()
    {
    }

    void Door::Init()
    {
    }

    void Door::Release()
    {
        Obstacle::Release();
        SafeDelete(col);
    }

    void Door::Update()
    {
        Obstacle::Update();
        col->Update();
    }

    void Door::LateUpdate()
    {
    }

    void Door::Render()
    {
        Obstacle::Render();
        col->Render();
    }

    void Door::Spawn(const Vector2 wpos, const DirState dir)
    {
        SetPos(wpos);
        idle->frame.y = dir;
        idle->isVisible = true;
        SOUND->Play("DoorClosed");
    }

    void Door::Disappear()
    {
        SetPos(DEFAULTSPAWN);
        idle->isVisible = false;
        SOUND->Play("DoorClosed");
    }
}