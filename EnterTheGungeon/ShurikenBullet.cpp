#include "stdafx.h"

namespace Gungeon
{
    ShurikenBullet::ShurikenBullet()
    {
        Init();

        float scaleFactor = 3.0f;
        col->scale = Vector2(64.0f / 4.0f, 16.0f) * scaleFactor;
        col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        SetPos(DEFAULTSPAWN);

        idle = make_shared<ObImage>(L"EnterTheGungeon/boss_1/Bullet_3.png");
        idle->isVisible = false;
        idle->scale = col->scale;
        idle->maxFrame.x = 4;
        idle->ChangeAnim(AnimState::loop, 0.2f);
        idle->SetParentRT(*col);

        float bombScaleFactor = 2.5f;
        hitBomb = make_shared<Effect>();
        hitBomb->idle = make_shared<ObImage>(L"EnterTheGungeon/boss_1/HitBomb.png");
        hitBomb->idle->maxFrame.x = 4;
        hitBomb->idle->scale = Vector2(88.0f / 4.0f, 22.0f) * bombScaleFactor;
        hitBomb->idle->isVisible = false;
    }

    void ShurikenBullet::Init()
    {
        Bullet::Init();
        scalar = 400.0f;
        damage = 1;
        angle = 0.0f;
        atkAngle = 0.0f;
        ShurikenBulletState = ShurikenBulletState::rotate;
    }

    void ShurikenBullet::Update()
    {
        Bullet::Update();
        if (isFired)
        {
            Vector2 velocity;
            switch (ShurikenBulletState)
            {
            case Gungeon::ShurikenBulletState::rotate:
                col->rotation += RANDOM.Float(0.008f, 0.02f);
                col->rotation2 += RANDOM.Float(0.008f, 0.02f);
                if (TIMER.GetTick(timeTargeting, intervalTargeting))
                    ShurikenBulletState = ShurikenBulletState::targeting;
                break;

            case Gungeon::ShurikenBulletState::targeting:
                if (TIMER.GetTick(timeAttack, intervalAttack))
                {
                    wposBeforeRot = col->GetWorldPos();
                    col->rotation = 0.0f;
                    col->rotation2 = 0.0f;
                    col->Update();
                    col->SetWorldPos(wposBeforeRot);
                    ShurikenBulletState = ShurikenBulletState::attack;
                    
                    SOUND.Play("BulletKingThrow");
                }
                break;

            case Gungeon::ShurikenBulletState::attack:
                velocity = moveDir * scalar * DELTA;
                col->MoveWorldPos(velocity);
                break;
            }
        }

        idle->Update();
        hitBomb->Update();
    }
}