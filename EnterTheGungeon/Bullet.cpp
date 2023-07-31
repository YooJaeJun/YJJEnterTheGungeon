#include "stdafx.h"

namespace Gungeon
{
    Bullet::Bullet()
    {
        col = make_shared<ObCircle>();
        col->isVisible = false;
        col->isFilled = false;
        Bullet::Init();
    }

    void Bullet::Init()
    {
        moveDir = Vector2(0.0f, 0.0f);
        scalar = 120.0f;
        col->rotation = 0.0f;
        col->rotationRelative = 0.0f;
        SetPos(DEFAULTSPAWN);
        timeLife = 0.0f;
        intervalLife = 10.0f;
    }

    void Bullet::Update()
    {
        if (isFired)
        {
            SetLastPos();
            Character::Update();
            Fire();
        }

        idle->Update();
        hitBomb->Update();
    }

    void Bullet::Update(const bool notRotation)
    {
        if (isFired)
        {
            Character::Update(notRotation);
            Fire();
        }

        idle->Update(false);
        hitBomb->Update();
    }

    void Bullet::LateUpdate()
    {
    }

    void Bullet::Render()
    {
        if (isFired)
        {
            idle->Render(); // RENDER->push(idle);
            Character::Render();
        }
        hitBomb->Render();
    }

    void Bullet::Spawn(const Vector2& coord)
    {
        SetPos(coord);
        isFired = true;
        idle->isVisible = true;
    }

    void Bullet::Spawn(const Vector2& coord, const Vector2& fireDir)
    {
        Spawn(coord);
        col->rotation = Utility::DirToRadian(fireDir);
        moveDir = fireDir;
    }

    void Bullet::Fire()
    {
        moveDir.Normalize();
        const Vector2 velocity = moveDir * scalar * DELTA;
        col->MoveWorldPos(velocity);
    }

    void Bullet::Hit(const float damage)
    {
        hitBomb->Spawn(idle->GetWorldPos());
        SOUND.Play("Bomb");
        Reload();
    }

    void Bullet::Reload()
    {
        SetPos(DEFAULTSPAWN);
        idle->isVisible = false;
        Update();
        isFired = false;
    }
}