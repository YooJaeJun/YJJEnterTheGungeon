#include "stdafx.h"

namespace Gungeon
{
    EnemyBullet::EnemyBullet()
    {
        Init();

        col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        SetPos(DEFAULTSPAWN);

        float scaleFactor = 3.5f;
        col->scale = Vector2(8.0f, 8.0f) * scaleFactor;
        SetPos(DEFAULTSPAWN);
        idle = make_shared<ObImage>(L"EnterTheGungeon/enemy_1/Bullet_0.png");
        idle->isVisible = false;
        idle->scale = col->scale;
        idle->SetParentRT(*col);

        float bombScaleFactor = 1.5f;
        hitBomb = make_shared<Effect>();
        hitBomb->idle = make_shared<ObImage>(L"EnterTheGungeon/enemy_1/HitBomb.png");
        hitBomb->idle->maxFrame.x = 4;
        hitBomb->idle->scale = Vector2(88.0f / 4.0f, 22.0f) * bombScaleFactor;
        hitBomb->idle->isVisible = false;
    }

    void EnemyBullet::Init()
    {
        Bullet::Init();

        scalar = 400.0f;
        damage = 1;
    }
}