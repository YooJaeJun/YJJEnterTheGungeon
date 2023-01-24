#include "stdafx.h"

namespace Gungeon
{
    BossBullet::BossBullet()
    {
        Init();

        float scaleFactor = 2.5f;
        col->scale = Vector2(16.0f, 16.0f) * scaleFactor;
        col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        SetPos(DEFAULTSPAWN);

        idle = make_shared<ObImage>(L"EnterTheGungeon/boss_1/Bullet_0.png");
        idle->isVisible = false;
        idle->scale = col->scale;
        // idle->maxFrame.x = 4;
        // idle->ChangeAnim(AnimState::loop, 0.1f);
        idle->SetParentRT(*col);

        float bombScaleFactor = 2.5f;
        hitBomb = make_shared<Effect>();
        hitBomb->idle = make_shared<ObImage>(L"EnterTheGungeon/boss_1/HitBomb.png");
        hitBomb->idle->maxFrame.x = 4;
        hitBomb->idle->scale = Vector2(88.0f / 4.0f, 22.0f) * bombScaleFactor;
        hitBomb->idle->isVisible = false;
    }

    void BossBullet::Init()
    {
        Bullet::Init();
        scalar = 400.0f;
        damage = 1;
        angle = 0.0f;
        atkAngle = 0.0f;
    }
}