#include "stdafx.h"

namespace Gungeon
{
    TrailBullet::TrailBullet()
    {
        Init();

        float scaleFactor = 3.0f;
        col->scale = Vector2(80.0f / 4.0f, 17.0f) * scaleFactor;
        col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        SetPos(DEFAULTSPAWN);

        idle = make_shared<ObImage>(L"EnterTheGungeon/boss_1/Bullet_1.png");
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


        trails.resize(trailNum);

        for (auto& trail : trails)
        {
            trail = make_shared<ObImage>(L"EnterTheGungeon/boss_1/Bullet_2.png");
            trail->isVisible = false;
            trail->scale = col->scale;
        }
    }

    void TrailBullet::Init()
    {
        Bullet::Init();
        scalar = 400.0f;
        damage = 1;
        angle = 0.0f;
        atkAngle = 0.0f;

        timeTrail = 0.02f;
        trailNum = 10;
        timeSpawnTrail = 0.0f;
    }

    void TrailBullet::Update()
    {
        Bullet::Update();

        if (TIMER.GetTick(timeSpawnTrail, timeTrail))
        {
            trails[0]->isVisible = true;
            trails[0]->SetWorldPos(Pos() == Vector2(0.0f, 0.0f) ? DEFAULTSPAWN : Pos());
            trails[0]->rotation = col->rotation;
            trails[0]->scale = Vector2(30.0f, 30.0f);
            trails.emplace_back(trails[0]);
            trails.pop_front();
        }

        int idx = 0;
        for (auto& elem : trails)
        {
            if (idx == trails.size() - 1) elem->color = Color(1.0f, 0.6f, 0.5f);
            else elem->color = Color(0.7f, 0.5f, 0.3f); 
            elem->Update();
            idx++;
        }
    }

    void TrailBullet::Render()
    {
        if (isFired)
            for (auto& elem : trails)
                elem->Render();
        Bullet::Render();
    }
}