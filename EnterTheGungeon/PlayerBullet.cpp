#include "stdafx.h"

namespace Gungeon
{
    PlayerBullet::PlayerBullet()
    {
        Init();

        float scaleFactor = 1.5f;
        col->scale = Vector2(16.0f, 16.0f) * scaleFactor;
        col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        SetPos(DEFAULTSPAWN);

        idle = new ObImage(L"EnterTheGungeon/player_1/Bullet_0.png");
        idle->isVisible = false;
        idle->scale = col->scale;
        idle->SetParentRT(*col);

        float bombScaleFactor = 1.5f;
        hitBomb = new Effect;
        hitBomb->idle = new ObImage(L"EnterTheGungeon/player_1/HitBomb.png");
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