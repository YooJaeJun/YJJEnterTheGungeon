#include "stdafx.h"

namespace Gungeon
{
    PlayerBullet::PlayerBullet()
    {
        Init();

        constexpr float scaleFactor = 1.5f;
        col->scale = Vector2(16.0f, 16.0f) * scaleFactor;
        col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        SetPos(DEFAULTSPAWN);

        idle = make_shared<ObImage>(L"EnterTheGungeon/player_1/Bullet_0.png");
        idle->isVisible = false;
        idle->scale = col->scale;
        idle->SetParentRT(*col);

        constexpr float bombScaleFactor = 1.5f;
        hitBomb = make_shared<Effect>();
        hitBomb->idle = make_shared<ObImage>(L"EnterTheGungeon/player_1/HitBomb.png");
        hitBomb->idle->maxFrame.x = 4;
        hitBomb->idle->scale = Vector2(88.0f / 4.0f, 22.0f) * bombScaleFactor;
        hitBomb->idle->isVisible = false;
    }

    void PlayerBullet::Init()
    {
        Bullet::Init();

        scalar = 600.0f;
        damage = 1;
    }
}