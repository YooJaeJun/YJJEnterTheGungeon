#include "stdafx.h"

namespace Gungeon
{
    GravityBullet::GravityBullet()
    {
        Init();

        float scaleFactor = 2.0f;
        col->scale = Vector2(242.0f / 11.0f, 22.0f) * scaleFactor;
        col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        SetPos(DEFAULTSPAWN);

        idle = make_shared<ObImage>(L"EnterTheGungeon/boss_1/Bullet_4.png");
        idle->isVisible = false;
        idle->scale = col->scale;
        idle->maxFrame.x = 11;
        idle->ChangeAnim(AnimState::loop, 0.1f);
        idle->SetParentRT(*col);

        float bombScaleFactor = 2.5f;
        hitBomb = make_shared<Effect>();
        hitBomb->idle = make_shared<ObImage>(L"EnterTheGungeon/boss_1/HitBomb.png");
        hitBomb->idle->maxFrame.x = 4;
        hitBomb->idle->scale = Vector2(88.0f / 4.0f, 22.0f) * bombScaleFactor;
        hitBomb->idle->isVisible = false;
    }

    void GravityBullet::Init()
    {
        Bullet::Init();

        scalar = 600.0f;
        damage = 1;
        gravity = 400.0f;
    }

    void GravityBullet::Update()
    {
        if (isFired)
        {
            SetLastPos();
            Character::Update();

            moveDir.Normalize();
            Vector2 velocity = moveDir * scalar * DELTA + gravity * DOWN * DELTA;
            col->MoveWorldPos(velocity);

            gravity += 600.0f * DELTA;
        }

        idle->Update();
        hitBomb->Update();
    }

    void GravityBullet::Spawn(const Vector2& coord, const Vector2& fireDir)
    {
        Bullet::Spawn(coord, fireDir);
        gravity = -400.0f;
    }
}