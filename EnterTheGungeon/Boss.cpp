#include "stdafx.h"

namespace Gungeon
{
    Boss::Boss()
    {
        Init();
    }

    void Boss::Init()
    {
        InitVar();
        InitSelf();
        InitBullet();
        InitItem();
    }

    void Boss::InitVar()
    {
        pattern = BossPattern::none;

        name = L"ÃÑÅº Å·";
        desc = L"³³À¸·Î µÈ ¿ÕÁÂ¿¡ ¾É¾Æ";
        curHp = maxHp = 150;
        scalar = 120.0f;

        intervalAnim[(int)State::idle] = 0.5f;
        intervalAnim[(int)State::walk] = 0.5f;
        intervalAnim[(int)State::attack] = 0.2f;
        intervalAnim[(int)State::die] = 0.2f;
        intervalAnim[(int)State::cinematic] = 0.3f;

        intervalHit = 0.2f;
        timeAttackStart = 0.0f;
        intervalAttackStart = 0.5f;

        intervalFire[(int)BossPattern::none] = 0.0f;
        intervalFire[(int)BossPattern::circular] = 1.5f;
        intervalFire[(int)BossPattern::string] = 2.0f;
        intervalFire[(int)BossPattern::tornado] = 0.0f;
        intervalFire[(int)BossPattern::spiral] = 0.05f;
        intervalFire[(int)BossPattern::trail] = 0.6f;
        intervalFire[(int)BossPattern::miro] = 0.3f;
        intervalFire[(int)BossPattern::brute] = 0.2f;
        intervalFire[(int)BossPattern::shuriken] = 0.2f;
        intervalFire[(int)BossPattern::gravity] = 0.3f;
        intervalFire[(int)BossPattern::wave] = 0.5f;

        intervalEnd[(int)BossPattern::none] = 0.0f;
        intervalEnd[(int)BossPattern::circular] = 5.8f;
        intervalEnd[(int)BossPattern::string] = 11.5f;
        intervalEnd[(int)BossPattern::tornado] = 12.0f;
        intervalEnd[(int)BossPattern::spiral] = 6.0f;
        intervalEnd[(int)BossPattern::trail] = 8.0f;
        intervalEnd[(int)BossPattern::miro] = 13.0f;
        intervalEnd[(int)BossPattern::brute] = 4.0f;
        intervalEnd[(int)BossPattern::shuriken] = 6.0f;
        intervalEnd[(int)BossPattern::gravity] = 12.0f;
        intervalEnd[(int)BossPattern::wave] = 10.0f;

        candidateStringCount = 4;
        candidateString = {
            "Yoo Jae Jun",
            "Lee Cha Yeon",
            "DirectX 11",
            "You Die"
        };

        miro = {
            "11111000011111111111111111111111111",
            "11111000011111111111111111111111111",
            "11111000011111111111111111111111111",
            "11111000011111111111111111111111111",
            "11111000000000000000000000000011111",
            "11111000000000000000000000000011111",
            "11111000000000000000000000000011111",
            "11111000000000000000000000000011111",
            "11111111111111111111111111000011111",
            "11111111111111111111111111000011111",
            "11111111111111111111111111000011111",
            "11111111111111111111111111000011111",
            "11111111111111111111111111000011111",
            "11111111111111111111111111000011111",
            "11111111111111111111111111000011111",
            "11111100000000000011111111000011111",
            "11111100000000000011111111000011111",
            "11111100000000000011111111000011111",
            "11111100000000000011111111000011111",
            "11111100001111000000000000000011111",
            "11111100001111000000000000000011111",
            "11111100001111000000000000000011111",
            "11111100001111000000000000000011111",
            "11111100001111111111111111111111111",
            "11111100001111111111111111111111111",
            "11111100001111111111111111111111111",
            "11111100001111111111111111111111111",
            "11111100001111111111111111111111111",
            "11111100000000000000000000111111111",
            "11111100000000000000000000111111111",
            "11111100000000000000000000111111111",
            "11111111110000000000000000111111111",
            "11111111110000000000000000111111111",
            "11111111110000000000000000111111111",
            "11111111110000000000000000111111111",
        };
        float scaleFactor = 4.0f;
        spawnPlayerByForce = new Effect;
        spawnPlayerByForce->idle = new ObImage(L"EnterTheGungeon/Level/Spawn_Player_By_Force.png");
        spawnPlayerByForce->idle->isVisible = false;
        spawnPlayerByForce->idle->maxFrame.x = 4;
        spawnPlayerByForce->idle->scale = Vector2(188.0f / 4.0f, 44.0f) * scaleFactor;

        flagRandomPattern = true;
    }

    void Boss::InitSelf()
    {
        state = State::die;

        float scaleFactor = 3.0f;
        col = new ObCircle;
        col->isVisible = false;
        col->isFilled = false;
        col->scale = Vector2(25.0f, 25.0f) * scaleFactor * 3.0f;
        col->color = Color(1.0f, 1.0f, 1.0f);
        SetPos(DEFAULTSPAWN);

        colTile = new ObRect;
        colTile->isVisible = false;
        colTile->scale = Vector2(col->scale.x / 2.0f, col->scale.y / 4.0f);
        colTile->SetParentRT(*col);
        colTile->SetLocalPosY(col->GetWorldPos().y - col->scale.y / 2.0f + 10.0f);
        colTile->isFilled = false;
        colTile->color = Color(1.0f, 1.0f, 1.0f, 1.0f);

        respawn = new ObImage(L"EnterTheGungeon/Boss_1/Spawn.png");
        respawn->isVisible = true;
        respawn->maxFrame.x = 20;
        respawn->scale = Vector2(800.0f / 16.0f, 44.0f) * scaleFactor;
        respawn->SetParentRT(*col);

        idle = new ObImage(L"EnterTheGungeon/Boss_1/Idle.png");
        idle->isVisible = true;
        idle->maxFrame = Int2(2, 8);
        idle->scale = Vector2(80.0f / 2.0f, 352.0f / 8.0f) * scaleFactor;
        idle->ChangeAnim(AnimState::loop, intervalAnim[(int)State::idle]);
        idle->SetParentRT(*col);

        walk = new ObImage(L"EnterTheGungeon/Boss_1/Walk.png");
        walk->isVisible = false;
        walk->maxFrame = Int2(2, 8);
        walk->scale = Vector2(80.0f / 2.0f, 352.0f / 8.0f) * scaleFactor;
        walk->ChangeAnim(AnimState::loop, intervalAnim[(int)State::walk]);
        walk->SetParentRT(*col);

        attack1Start = new ObImage(L"EnterTheGungeon/Boss_1/Attack_1_Start.png");
        attack1Start->isVisible = false;
        attack1Start->maxFrame.x = 1;
        attack1Start->scale = Vector2(40.0f, 44.0f) * scaleFactor;
        attack1Start->ChangeAnim(AnimState::loop, intervalAnim[(int)State::attack]);
        attack1Start->SetParentRT(*col);

        attack1 = new ObImage(L"EnterTheGungeon/Boss_1/Attack_1.png");
        attack1->isVisible = false;
        attack1->maxFrame.x = 2;
        attack1->scale = Vector2(80.0f / 2.0f, 44.0f) * scaleFactor;
        attack1->ChangeAnim(AnimState::loop, intervalAnim[(int)State::attack]);
        attack1->SetParentRT(*col);

        attack2 = new ObImage(L"EnterTheGungeon/Boss_1/Attack_2.png");
        attack2->isVisible = false;
        attack2->maxFrame.x = 6;
        attack2->scale = Vector2(240.0f / 6.0f, 44.0f) * scaleFactor;
        attack2->ChangeAnim(AnimState::loop, intervalAnim[(int)State::attack]);
        attack2->SetParentRT(*col);

        attack3 = new ObImage(L"EnterTheGungeon/Boss_1/Attack_3.png");
        attack3->isVisible = false;
        attack3->maxFrame.x = 5;
        attack3->scale = Vector2(200.0f / 5.0f, 44.0f) * scaleFactor;
        attack3->ChangeAnim(AnimState::loop, intervalAnim[(int)State::attack]);
        attack3->SetParentRT(*col);

        hit = new ObImage(L"EnterTheGungeon/Boss_1/Hit.png");
        hit->isVisible = false;
        hit->maxFrame.x = 2;
        hit->scale = Vector2(72.0 / 2.0f, 38.0f) * scaleFactor;
        hit->ChangeAnim(AnimState::loop, intervalHit);
        hit->SetParentRT(*col);

        die = new ObImage(L"EnterTheGungeon/Boss_1/Die.png");
        die->isVisible = false;
        die->maxFrame.x = 2;
        die->scale = Vector2(72.0f / 2.0f, 40.0f) * scaleFactor;
        die->ChangeAnim(AnimState::loop, intervalAnim[(int)State::die]);
        die->SetParentRT(*col);

        chairIdle = new ObImage(L"EnterTheGungeon/Boss_1/Chair_Idle.png");
        chairIdle->isVisible = true;
        chairIdle->maxFrame = Int2(2, 8);
        chairIdle->scale = Vector2(168.0f / 2.0f, 616.0f / 8.0f) * scaleFactor;
        chairIdle->ChangeAnim(AnimState::stop, 0.5f);
        chairIdle->SetParentRT(*col);

        chairWalk = new ObImage(L"EnterTheGungeon/Boss_1/Chair_Walk.png");
        chairWalk->isVisible = false;
        chairWalk->maxFrame = Int2(2, 8);
        chairWalk->scale = Vector2(168.0f / 2.0f, 616.0f / 8.0f) * scaleFactor;
        chairWalk->ChangeAnim(AnimState::loop, 0.5f);
        chairWalk->SetParentRT(*col);

        chairAttack1Start = new ObImage(L"EnterTheGungeon/Boss_1/Chair_Attack_1_Start.png");
        chairAttack1Start->isVisible = false;
        chairAttack1Start->maxFrame.x = 1;
        chairAttack1Start->scale = Vector2(64.0f, 78.0f) * scaleFactor;
        chairAttack1Start->ChangeAnim(AnimState::loop, intervalAnim[(int)State::attack]);
        chairAttack1Start->SetParentRT(*col);

        chairAttack1 = new ObImage(L"EnterTheGungeon/Boss_1/Chair_Attack_1.png");
        chairAttack1->isVisible = false;
        chairAttack1->maxFrame.x = 2;
        chairAttack1->scale = Vector2(120.0f / 2.0f, 74.0f) * scaleFactor;
        chairAttack1->ChangeAnim(AnimState::loop, 0.05f);
        chairAttack1->SetParentRT(*col);

        chairAttack2 = new ObImage(L"EnterTheGungeon/Boss_1/Chair_Attack_2.png");
        chairAttack2->isVisible = false;
        chairAttack2->maxFrame.x = 3;
        chairAttack2->scale = Vector2(204.0f / 3.0f, 79.0f) * scaleFactor;
        chairAttack2->ChangeAnim(AnimState::loop, intervalAnim[(int)State::attack]);
        chairAttack2->SetParentRT(*col);

        chairAttack3 = new ObImage(L"EnterTheGungeon/Boss_1/Chair_Attack_3.png");
        chairAttack3->isVisible = false;
        chairAttack3->maxFrame.x = 3;
        chairAttack3->scale = Vector2(204.0f / 3.0f, 79.0f) * scaleFactor;
        chairAttack3->ChangeAnim(AnimState::loop, intervalAnim[(int)State::attack]);
        chairAttack3->SetParentRT(*col);

        chairDie = new ObImage(L"EnterTheGungeon/Boss_1/Chair_Die.png");
        chairDie->isVisible = false;
        chairDie->maxFrame.x = 1;
        chairDie->scale = Vector2(73.0f, 75.0f) * scaleFactor;
        chairDie->ChangeAnim(AnimState::once, intervalAnim[(int)State::die]);
        chairDie->SetParentRT(*col);

        float bombScaleFactor = 2.5f;
        for (auto& elem : dieBomb)
        {
            elem = new Effect;
            elem->idle = new ObImage(L"EnterTheGungeon/boss_1/HitBomb.png");
            elem->idle->isVisible = false;
            elem->idle->maxFrame.x = 4;
            elem->idle->scale = Vector2(88.0f / 4.0f, 22.0f) * bombScaleFactor;
            elem->idle->ChangeAnim(AnimState::loop, intervalBombAnim);
            elem->idle->SetParentRT(*chairIdle);
        }

        firePosTargeting = new ObCircle;
        firePosTargeting->isVisible = false;
        firePosTargeting->scale = Vector2(10.0f, 10.0f);
        firePosTargeting->isFilled = false;
        firePosTargeting->color = Color(1.0f, 1.0f, 1.0f);
        firePosTargeting->SetParentRT(*col);
        firePosTargeting->SetLocalPos(Vector2(100.0f, 0.0f));

        firePosCannon = new ObCircle;
        firePosCannon->isVisible = false;
        firePosCannon->scale = Vector2(10.0f, 10.0f);
        firePosCannon->isFilled = false;
        firePosCannon->color = Color(1.0f, 1.0f, 1.0f);
        firePosCannon->SetParentRT(*col);
        firePosCannon->SetLocalPos(Vector2(0.0f, 100.0f));

        float shadowScaleFactor = scaleFactor * 2.0f;
        shadow = new ObImage(L"EnterTheGungeon/boss_1/Shadow_1.png");
        shadow->scale = Vector2(12.0f, 4.0f) * shadowScaleFactor;
        shadow->SetParentRT(*col);
        shadow->SetWorldPosY(-55.0f);

        float hpGuageFactor = 1.5f;

        hpGuageBar = new UI;
        hpGuageBar->img = new ObImage(L"EnterTheGungeon/boss_1/Hp_GuageBar.png");
        hpGuageBar->imgSize = Vector2(400.0f, 30.0f) * hpGuageFactor;   // °ÔÀÌÁö´Â imgSize ÇÊ¿ä
        hpGuageBar->img->scale = hpGuageBar->imgSize;
        hpGuageBar->anchor = DirState::dirB;
        hpGuageBar->Spawn(Vector2(-hpGuageBar->imgSize.x / 2.0f, 40.0f));
        hpGuageBar->img->pivot = OFFSET_L;
        hpGuageBar->img->space = Space::screen;

        hpGuage = new UI;
        hpGuage->img = new ObImage(L"EnterTheGungeon/boss_1/Hp_Guage.png");
        hpGuage->imgSize = Vector2(330.0f, 16.0f) * hpGuageFactor;   // °ÔÀÌÁö´Â imgSize ÇÊ¿ä
        hpGuage->img->scale = hpGuage->imgSize;
        hpGuage->anchor = DirState::dirB;
        hpGuage->Spawn(Vector2(-hpGuage->imgSize.x / 2.0f, 40.0f));
        hpGuage->img->pivot = OFFSET_L;
        hpGuage->img->space = Space::screen;

        cutScene = new UI;
        cutScene->img = new ObImage(L"EnterTheGungeon/Level/CutScene.png");
        cutScene->img->maxFrame.x = 2;
        cutScene->img->scale = Vector2(app.GetWidth(), app.GetHeight());
        cutScene->img->isVisible = false;
        cutScene->img->space = Space::screen;
    }

    void Boss::InitBullet()
    {
        switch (pattern)
        {
        case Gungeon::BossPattern::none:
            break;
        case Gungeon::BossPattern::circular:
            InitCircular();
            break;
        case Gungeon::BossPattern::string:
            InitString();
            break;
        case Gungeon::BossPattern::tornado:
            InitTornado();
            break;
        case Gungeon::BossPattern::spiral:
            InitSpiral();
            break;
        case Gungeon::BossPattern::trail:
            InitTrail();
            break;
        case Gungeon::BossPattern::miro:
            InitMiro();
            break;
        case Gungeon::BossPattern::brute:
            InitBrute();
            break;
        case Gungeon::BossPattern::shuriken:
            InitShuriken();
            break;
        case Gungeon::BossPattern::gravity:
            InitGravity();
            break;
        case Gungeon::BossPattern::wave:
            InitWave();
            break;
        }//switch
    }

    void Boss::InitItem()
    {
        float scaleFactor = 0.5f;
        dropItem = new Item;
        dropItem->col = new ObCircle;
        dropItem->col->scale = Vector2(40.0f, 40.0f) * scaleFactor;
        dropItem->col->isFilled = false;
        dropItem->SetPos(DEFAULTSPAWN);
        dropItem->idle = new ObImage(L"EnterTheGungeon/player_1/UI_Gold.png");
        dropItem->idle->scale = Vector2(40.0f, 40.0f) * scaleFactor;
        dropItem->idle->SetParentRT(*dropItem->col);
        dropItem->idle->isVisible = false;
    }

    void Boss::Release()
    {
        Unit::Release();
        SafeDelete(chairIdle);
        SafeDelete(chairWalk);
        SafeDelete(attack1Start);
        SafeDelete(attack1);
        SafeDelete(attack2);
        SafeDelete(attack3);
        SafeDelete(chairAttack1Start);
        SafeDelete(chairAttack1);
        SafeDelete(chairAttack2);
        SafeDelete(chairAttack3);
        SafeDelete(chairDie);
        SafeDelete(hpGuage);
        SafeDelete(hpGuageBar);
    }

    void Boss::Update()
    {
        Unit::SetTarget(weapon);
        Unit::Update();

        ImGui::Checkbox("Random Pattern", &flagRandomPattern);

        switch (state)
        {
        case Gungeon::State::cinematic:
            break;
        case Gungeon::State::idle:
            StartAttack();
            if (false == (moveDir.x == 0.0f && moveDir.y == 0.0f))
            {
                chairIdle->isVisible = false;
                chairWalk->isVisible = true;
            }
            Idle();
            break;
        case Gungeon::State::walk:
            StartAttack();
            if (moveDir.x == 0.0f && moveDir.y == 0.0f)
            {
                chairWalk->isVisible = false;
                chairIdle->isVisible = true;
            }
            Walk();
            break;
        case Gungeon::State::attack:
            Attack();
            break;
        case Gungeon::State::die:
            Die();
            break;
        }

        for (auto& elem : bullet)
        {
            if (elem)
            {
                elem->Update();
            }
        }

        attack1Start->Update();
        attack1->Update();
        attack2->Update();
        attack3->Update();

        chairIdle->Update();
        chairWalk->Update();
        chairAttack1Start->Update();
        chairAttack1->Update();
        chairAttack2->Update();
        chairAttack3->Update();
        chairDie->Update();

        firePosTargeting->Update();
        firePosCannon->Update();
        hpGuageBar->Update();
        hpGuage->Update();
        dropItem->Update();
        spawnPlayerByForce->Update();

        for (auto& elem : dieBomb)
        {
            elem->Update();
        }

        cutScene->Update();
    }

    void Boss::LateUpdate()
    {
    }

    void Boss::Render()
    {
        chairIdle->Render();
        chairWalk->Render();
        chairAttack1Start->Render();
        chairAttack1->Render();
        chairAttack2->Render();
        chairAttack3->Render();
        chairDie->Render();

        Unit::Render();
        attack1Start->Render();
        attack1->Render();
        attack2->Render();
        attack3->Render();

        for (auto& elem : bullet)
        {
            elem->Render();
        }

        firePosTargeting->Render();
        firePosCannon->Render();
        dropItem->Render();
        spawnPlayerByForce->Render();

        for (auto& elem : dieBomb)
        {
            elem->Render();
        }

        if (state != State::cinematic && state != State::die)
        {
            if (isUIRendering)
            {
                hpGuageBar->Render();
                hpGuage->Render();

                DWRITE->RenderText(name,
                    RECT{ (long)app.GetHalfWidth() - 280, (long)app.GetHeight() - 100,
                    (long)app.GetWidth(), (long)app.GetHeight() },
                    35.0f,
                    L"PF½ºÅ¸´õ½ºÆ®");
            }
        }

        cutScene->Render();
    }

    void Boss::ResizeScreen()
    {
        hpGuageBar->Spawn(Vector2(-hpGuageBar->imgSize.x / 2.0f, 40.0f));
        hpGuage->Spawn(Vector2(-hpGuage->imgSize.x / 2.0f, 40.0f));
        
        cutScene->img->scale = Vector2(app.GetWidth(), app.GetHeight());
    }

    void Boss::Idle()
    {
        Hitting();
    }

    void Boss::Walk()
    {
        moveDir = targetDir;
        Hitting();
    }

    void Boss::Attack()
    {
        Hitting();

        switch (attackState)
        {
        case Gungeon::BossAttackState::none:
            break;
        case Gungeon::BossAttackState::start:
            idle->isVisible = false;
            walk->isVisible = false;
            chairIdle->isVisible = false;
            hit->isVisible = false;

            switch (pattern)
            {
            case Gungeon::BossPattern::tornado:
            case Gungeon::BossPattern::brute:
            case Gungeon::BossPattern::gravity:
                attack1->isVisible = true;
                chairAttack1->isVisible = true;
                break;

            case Gungeon::BossPattern::miro:
            case Gungeon::BossPattern::spiral:
            case Gungeon::BossPattern::shuriken:
            case Gungeon::BossPattern::wave:
                attack2->isVisible = true;
                chairAttack2->isVisible = true;
                break;

            case Gungeon::BossPattern::circular:
            case Gungeon::BossPattern::string:
            case Gungeon::BossPattern::trail:
                attack3->isVisible = true;
                chairAttack3->isVisible = true;
                break;
            }

            attackState = BossAttackState::loop;
            break;

        case Gungeon::BossAttackState::loop:

            hit->isVisible = false;
            idle->isVisible = false;
            walk->isVisible = false;

            switch (pattern)
            {
            case Gungeon::BossPattern::tornado:
            case Gungeon::BossPattern::brute:
            case Gungeon::BossPattern::gravity:
                if (attack1->frame.x == attack1->maxFrame.x - 1)
                {
                    SOUND->Play("BulletKingSpin");
                }
                break;

            case Gungeon::BossPattern::miro:
            case Gungeon::BossPattern::spiral:
            case Gungeon::BossPattern::shuriken:
            case Gungeon::BossPattern::wave:
                if (attack2->frame.x == attack2->maxFrame.x - 1)
                {
                    SOUND->Play("BulletKingThrow");
                }
                break;

            case Gungeon::BossPattern::circular:
            case Gungeon::BossPattern::string:
            case Gungeon::BossPattern::trail:
                if (attack3->frame.x == attack3->maxFrame.x - 1)
                {
                    SOUND->Play("BulletKingChairDown");
                }
                break;
            }

            if (TIMER->GetTick(timeAttackEnd, intervalEnd[(int)pattern]))
            {
                attackState = BossAttackState::reload;
            }
            break;
        case Gungeon::BossAttackState::reload:
            idle->isVisible = true;
            attack1->isVisible = false;
            attack2->isVisible = false;
            attack3->isVisible = false;
            chairAttack1->isVisible = false;
            chairAttack2->isVisible = false;
            chairAttack3->isVisible = false;
            chairIdle->isVisible = true;

            HitBullet();

            attackState = BossAttackState::end;
            break;
        case Gungeon::BossAttackState::end:
            if (TIMER->GetTick(timeInit, intervalInit))
            {
                attackState = BossAttackState::none;
                state = State::walk;
            }
            break;
        default:
            break;
        }


        if (attackState != BossAttackState::reload &&
            attackState != BossAttackState::end)
        {
            UpdateBullet();
        }
    }

    void Boss::Die()
    {
        for (auto& elem : dieBomb)
        {
            if (false == realDie && 
                TIMER->GetTick(timeBombBetween, intervalBombBetween))
            {
                elem->idle->SetLocalPosX(RANDOM->Float(-chairIdle->scale.x / 2.0f, chairIdle->scale.x / 2.0f));
                elem->idle->SetLocalPosY(RANDOM->Float(-chairIdle->scale.y / 2.0f, chairIdle->scale.y / 2.0f));
                elem->idle->isVisible = true;
            }
        }

        if (false == realDie && 
            TIMER->GetTick(timeRealDie, 3.5f))
        {
            for (auto& elem : dieBomb)
            {
                elem->idle->isVisible = false;
                elem->idle->ChangeAnim(AnimState::stop, 0.1f);
            }

            die->ChangeAnim(AnimState::stop, 0.1f);
            die->color = Color(0.4f, 0.4f, 0.4f, 0.4f);
            chairIdle->isVisible = false;
            chairDie->isVisible = true;
            chairDie->color = Color(0.4f, 0.4f, 0.4f, 0.4f);
            realDie = true;

            SOUND->Play("Voice_Boss_RealDie");
        }
    }

    void Boss::StartHit(const int damage, const Vector2& dir)
    {
        pushedDir = dir;

        Unit::StartHit(damage);
        if (attackState == BossAttackState::loop)
        {
            hit->isVisible = false;
            hit->ChangeAnim(AnimState::stop, 0.1f);
        }
        else
        {
            hit->ChangeAnim(AnimState::once, 0.1f);

            int r = RANDOM->Int(0, 1);
            if (r)
            {
                SOUND->Stop("Voice_Boss_Hit1");
                SOUND->Play("Voice_Boss_Hit1");
            }
            else
            {
                SOUND->Stop("Voice_Boss_Hit2");
                SOUND->Play("Voice_Boss_Hit2");
            }
        }


        if (pushedDir.x < 0.0f)
        {
            hit->reverseLR = true;
        }
        else
        {
            hit->reverseLR = false;
        }
    }

    void Boss::Hitting()
    {
        if (isHit)
        {
            if (curHp <= 0.0f)
            {
                curHp = 0.0f;
                hpGuageBar->img->isVisible = false;
                hpGuage->img->isVisible = false;
                StartDie();
            }
            else
            {
                hpGuage->img->scale.x = (float)curHp / maxHp * hpGuage->imgSize.x;
                hpGuage->img->uv.z = hpGuage->img->scale.x / hpGuage->imgSize.x;
            }

            if (TIMER->GetTick(timeHit, 0.01f))
            {
                isHit = false;
            }
        }

        if (isHitAnim)
        {
            Color c = Color(RANDOM->Float(0.6f, 1.0f), 0.5f, 0.5f, RANDOM->Float(0.2f, 1.0f));
            idle->color = c;
            walk->color = c;
            hit->color = c;

            SetPosX(Pos().x + RANDOM->Float(-1.0f, 1.0f));
            SetPosY(Pos().y + RANDOM->Float(-1.0f, 1.0f));

            idle->isVisible = false;
            walk->isVisible = false;
            hit->isVisible = true;

            if (TIMER->GetTick(timeHitAnim, 0.4f))
            {
                Color c = Color(0.5f, 0.5f, 0.5f, 1.0f);
                idle->color = c;
                walk->color = c;
                hit->color = c;
                die->color = c;

                idle->isVisible = true;
                hit->isVisible = false;

                isHitAnim = false;

                pushedDir = Vector2(0.0f, 0.0f);
            }
        }
        else
        {
            idle->color.w = 1.0f;
            walk->color.w = 1.0f;
            hit->isVisible = false;
        }
    }

    void Boss::StartAttack()
    {
        if (flagRandomPattern)
        {
            if (state != State::cinematic &&
                attackState == BossAttackState::none &&
                TIMER->GetTick(timeAttackStart, intervalAttackStart))
            {
                intervalAttackStart = RANDOM->Float(1.0f, 3.0f);

                BossPattern randomPattern;
                do {
                    randomPattern = (BossPattern)RANDOM->Int(1, (int)BossPattern::max - 1);
                } while (pattern == randomPattern);

                ChangePattern(randomPattern);
                InitBullet();

                // test
                // pattern = BossPattern::gravity;
                // ChangePattern(pattern);
                // InitBullet();

                state = State::attack;
                attackState = BossAttackState::start;
            }
        }
        else
        {
            int curPattern = static_cast<int>(pattern);
            if (ImGui::SliderInt("Boss Pattern", &curPattern, 0, patternMax))
            {
                ChangePattern((BossPattern)curPattern);
                InitBullet();

                state = State::attack;
                attackState = BossAttackState::start;
            }
        }
    }

    void Boss::StartDie()
    {
        if (realDie) return;

        Unit::StartDie();

        die->ChangeAnim(AnimState::loop, intervalAnim[(int)State::die]);

        attack1Start->isVisible = false;
        attack1->isVisible = false;
        attack2->isVisible = false;
        attack3->isVisible = false;

        chairWalk->isVisible = false;
        chairAttack1Start->isVisible = false;
        chairAttack1->isVisible = false;
        chairAttack2->isVisible = false;
        chairAttack3->isVisible = false;

        hpGuageBar->img->isVisible = false;
        hpGuage->img->isVisible = false;

        chairIdle->isVisible = true;

        dropItem->Spawn(Vector2(Pos().x - 10.0f, Pos().y - 10.0f));
        dropItem->idle->isVisible = true;
        dropItem->state = State::idle;

        HitBullet();

        SOUND->Stop("BossBGM");
        SOUND->Play("FinalShot");
        SOUND->Play("Voice_Boss_Die");
    }

    void Boss::Spawn(const Vector2 wpos)
    {
        Unit::Spawn(wpos);

        InitVar();

        if (pushedDir.x < 0.0f)
        {
            die->reverseLR = true;
        }
        else
        {
            die->reverseLR = false;
        }

        dropItem->idle->isVisible = false;
        dropItem->state = State::die;
    }

    void Boss::HitBullet()
    {
        for (auto& elem : bullet)
        {
            if (elem->isFired)
            {
                elem->Hit(1);
            }
        }
    }

    void Boss::SpawnByForceInMiro(const Vector2 wpos)
    {
        col->SetWorldPos(Vector2(0.0f, 500.0f));
        spawnPlayerByForce->Spawn(wpos);
        spawnPlayerByForce->idle->ChangeAnim(AnimState::once, 0.15f);
        pushingPlayer = false;
        SOUND->Play("SpawnByForce");
    }

    void Boss::SpawnAnim()
    {
        idle->isVisible = false;
        chairIdle->ChangeAnim(AnimState::stop, 0.2f);
        respawn->isVisible = true;
        respawn->ChangeAnim(AnimState::once, intervalAnim[(int)State::cinematic]);
    }

    void Boss::SpawnAnimEnd()
    {
        respawn->isVisible = false;
        idle->isVisible = true;
        chairIdle->isVisible = true;
    }

    void Boss::CutSceneOn()
    {
        cutScene->img->isVisible = true;
        cutScene->img->ChangeAnim(AnimState::loop, 0.2f);
        SOUND->Play("BossIntro");
    }

    void Boss::CutSceneOff()
    {
        cutScene->img->isVisible = false;
        cutScene->img->ChangeAnim(AnimState::stop, 0.1f);
    }

    void Boss::UIOn(const bool on)
    {
        isUIRendering = on;
    }

    void Boss::ColToggle()
    {
        Character::ColToggle();
        firePosTargeting->isVisible ^= 1;
        firePosCannon->isVisible ^= 1;
        for (auto& bulletElem : bullet)
        {
            bulletElem->col->isVisible ^= 1;
        }
    }



    /// <summary>
    /// Pattern
    /// </summary>

    void Boss::ChangePattern(const BossPattern newPattern)
    {
        pattern = newPattern;
        curBulletIdx = 0;
        scalar = 120.0f;

        for (auto& elem : bullet)
        {
            SafeDelete(elem);
        }
        bullet.clear();
        
        way.clear();
    }

    void Boss::InitCircular()
    {
        bullet.resize(circularMax);
        bulletSpawnPos = firePosCannon->GetWorldPos();

        int idx = 0;
        for (auto& elem : bullet)
        {
            elem = new BossBullet;
            elem->Init();
            elem->moveDir.x = cos(idx * 6.0f * ToRadian);
            elem->moveDir.y = sin(idx * 6.0f * ToRadian);
            elem->scalar = 250.0f;
            elem->idle->color = Color(0.5f, 0.5f, 0.5f, 1.0f);
            idx++;
        }
    }

    void Boss::InitString()
    {
        stringBullet.inputString = candidateString[RANDOM->Int(0, candidateStringCount - 1)];
        stringBullet.SetStringBullet();
        bullet.resize(stringBullet.inputString.size() * 25);
        bulletSpawnPos = firePosTargeting->GetWorldPos();

        int size = stringBullet.inputString.size();

        int idx = 0;
        for (int r = 0; r < 5; r++)
        {
            for (int c = 0; c < 5; c++)
            {
                for (int i = 0; i < size; i++)
                {
                    bullet[idx] = new BossBullet;

                    if (stringBullet.outputAlphbets[i][r][c])
                    {
                        bullet[idx]->Init();
                        bullet[idx]->scalar = 200.0f + (r + 10.0f) * 15.0f;
                        bullet[idx]->angle = PI * 2 * (c + 1) / 5;
                        bullet[idx]->idle->color = Color(0.5f, 0.5f, 0.5f, 1.0f);
                    }

                    idx++;
                }
            }
        }
    }

    void Boss::InitTornado()
    {
        scalar *= 1.2f;

        bullet.resize(tornadoMax);

        int idx = 0;
        for (auto& elem : bullet)
        {
            elem = new BossBullet;
            elem->Init();
            elem->scalar = (idx + 1) * 3.0f;
            elem->col->SetParentRT(*col);
            elem->col->SetLocalPos(Vector2(80.0f + idx * 2.0f, 80.0f + idx * 2.0f));
            elem->idle->isVisible = true;
            elem->idle->color = Color(0.7f, 0.7f, 0.6f, 1.0f);
            idx++;
        }

        SOUND->Play("Voice_Boss_Tornado");
    }

    void Boss::InitSpiral()
    {
        bullet.resize(spiralMax);
        bulletSpawnPos = firePosCannon->GetWorldPos();

        int idx = 0;
        for (auto& elem : bullet)
        {
            elem = new BossBullet;
            elem->Init();
            elem->scalar = 100.0f + (idx + 10.0f) * 5.0f;
            elem->moveDir = Vector2(cos(idx * 360.0f / spiralMax * ToRadian), sin(idx * 360.0f / spiralMax * ToRadian));
            elem->idle->color = Color(0.5f, 0.5f, 0.5f, 1.0f);
            idx++;
        }
    }

    void Boss::InitTrail()
    {
        bullet.resize(trailMax);

        int idx = 0;
        for (auto& elem : bullet)
        {
            elem = new TrailBullet;
            elem->Init();
            elem->scalar = 700.0f;
            elem->idle->color = Color(0.7f, 0.5f, 0.3f, 1.0f);
            idx++;
        }
    }

    void Boss::InitMiro()
    {
        miroStart = true;
        pushingPlayer = true;

        bullet.clear();
        bulletSpawnPos = firePosCannon->GetWorldPos();
    }

    void Boss::InitBrute()
    {
        bullet.resize(bruteMax);
        bulletSpawnPos = firePosCannon->GetWorldPos();

        int idx = 0;
        bool flagAngleTrans = false;
        float factor = 0.0f;

        for (auto& elem : bullet)
        {
            elem = new BossBullet;
            elem->Init();
            elem->scalar = 600.0f;
            elem->idle->color = Color(0.5f, 0.5f, 0.5f, 1.0f);

            if (idx % 12 == 0) flagAngleTrans ^= 1;
            if (flagAngleTrans) factor = 0.0f;
            else factor = 15.0f;

            elem->moveDir = Vector2(cos((factor + idx * 30.0f) * ToRadian),
                sin((factor + idx * 30.0f) * ToRadian));
            idx++;
        }
    }

    void Boss::InitShuriken()
    {
        bullet.resize(shurikenMax);
        bulletSpawnPos = firePosTargeting->GetWorldPos();

        int idx = 0;
        for (auto& elem : bullet)
        {
            elem = new ShurikenBullet;
            elem->Init();
            elem->scalar = 600.0f;
            elem->col->SetParentRT(*col);
            elem->idle->color = Color(0.5f, 0.5f, 0.5f);
            elem->idle->ChangeAnim(AnimState::loop, RANDOM->Float(0.05f, 0.2f));
            idx++;
        }
    }

    void Boss::InitGravity()
    {
        bullet.resize(gravityMax);
        bulletSpawnPos = firePosCannon->GetWorldPos();

        int idx = 0;
        for (auto& elem : bullet)
        {
            elem = new GravityBullet;
            elem->Init();
            elem->scalar = 600.0f;
            elem->col->SetParentRT(*col);
            elem->idle->color = Color(0.5f, 0.5f, 0.5f);
            idx++;
        }
    }

    void Boss::InitWave()
    {
        bullet.resize(waveMax);
        bulletSpawnPos = firePosCannon->GetWorldPos();

        int idx = 0;
        for (auto& elem : bullet)
        {
            elem = new BossBullet;
            elem->Init();
            elem->scalar = 600.0f;
            elem->col->SetParentRT(*col);
            elem->idle->color = Color(0.5f, 0.5f, 0.5f);
            idx++;
        }
    }


    void Boss::UpdateBullet()
    {
        switch (pattern)
        {
        case Gungeon::BossPattern::circular:
            UpdateCircular();
            break;
        case Gungeon::BossPattern::string:
            UpdateString();
            break;
        case Gungeon::BossPattern::tornado:
            UpdateTornado();
            break;
        case Gungeon::BossPattern::spiral:
            UpdateSpiral();
            break;
        case Gungeon::BossPattern::trail:
            UpdateTrail();
            break;
        case Gungeon::BossPattern::miro:
            UpdateMiro();
            break;
        case Gungeon::BossPattern::brute:
            UpdateBrute();
            break;
        case Gungeon::BossPattern::shuriken:
            UpdateShuriken();
            break;
        case Gungeon::BossPattern::gravity:
            UpdateGravity();
            break;
        case Gungeon::BossPattern::wave:
            UpdateWave();
            break;
        }
    }

    void Boss::UpdateCircular()
    {
        if (TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::circular]))
        {
            int t = 60;
            while (t--)
            {
                bullet[curBulletIdx++]->Spawn(bulletSpawnPos);
                if (curBulletIdx >= circularMax) curBulletIdx = 0;
            }

            SOUND->Stop("BulletKingShot");
            SOUND->Play("BulletKingShot");
        }
    }

    void Boss::UpdateString()
    {
        int size = stringBullet.inputString.size();
        char* s = const_cast<char*>(stringBullet.inputString.c_str());
        bool flagChangeStringDirect = false;


        if (ImGui::InputText("String Danmaku", s, 27) ||
            bullet.size() != size * 25)
        {
            stringBullet.inputString = s;
            InitBullet();
            size = stringBullet.inputString.size();
            flagChangeStringDirect = true;
        }

        if (TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::string]))
        {
            if (false == flagChangeStringDirect)
            {
                InitBullet();
                size = stringBullet.inputString.size();
            }

            angleFactor = firePosTargeting->rotation2 - stringBullet.midForTargetFactor;

            int idx = 0;
            for (int r = 0; r < 5; r++)
            {
                for (int c = 0; c < 5; c++)
                {
                    for (int i = 0; i < size; i++)
                    {
                        if (stringBullet.outputAlphbets[i][r][c])
                        {
                            bullet[idx]->atkAngle = (bullet[idx]->angle / 60.0f) + (0.2f * i) + angleFactor;
                            bullet[idx]->moveDir = Vector2(cos(bullet[idx]->atkAngle), sin(bullet[idx]->atkAngle));
                        }

                        idx++;
                    }
                }
            }

            for (auto& elem : bullet)
            {
                if (elem->moveDir.x == 0.0f && elem->moveDir.y == 0.0f) continue;

                elem->Spawn(bulletSpawnPos);
            }

            SOUND->Stop("BulletKingShot");
            SOUND->Play("BulletKingShot");
        }
    }

    void Boss::UpdateTornado()
    {
        int idx = 0;
        for (auto& elem : bullet)
        {
            elem->isFired = true;
            elem->idle->isVisible = true;

            elem->col->SetLocalPos(Vector2(80.0f + idx * 2.0f, 80.0f + idx * 2.0f));
            elem->col->rotation += idx * 10.0f * ToRadian * DELTA;
            elem->col->rotation2 += idx * 4.0f * ToRadian * DELTA;

            idx++;
        }

        if (TIMER->GetTick(timeTornadoSound, intervalTornadoSound))
        {
            SOUND->Stop("BulletKingThrow");
            SOUND->Play("BulletKingThrow");
        }
    }

    void Boss::UpdateSpiral()
    {
        int idx = 0;
        bullet[curBulletIdx]->Spawn(bulletSpawnPos);

        if (TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::spiral]))
        {
            curBulletIdx++;
            if (curBulletIdx >= spiralMax)
            {
                curBulletIdx = 0;
            }

            SOUND->Stop("BulletKingThrow");
            SOUND->Play("BulletKingThrow");
        }
    }

    void Boss::UpdateTrail()
    {
        if (TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::trail]))
        {
            bulletSpawnPos = firePosTargeting->GetWorldPos();

            bullet[curBulletIdx]->moveDir.x = min(targetDir.x + RANDOM->Float(0.0f, 0.1f), 1.0f);
            bullet[curBulletIdx]->moveDir.y = min(targetDir.y + RANDOM->Float(0.0f, 0.1f), 1.0f);

            bullet[curBulletIdx]->Spawn(bulletSpawnPos, bullet[curBulletIdx]->moveDir);
            curBulletIdx++;

            SOUND->Stop("BulletKingShot");
            SOUND->Play("BulletKingShot");
        }
    }

    void Boss::UpdateMiro()
    {
        if (miroStart &&
            TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::miro]))
        {
            bullet.clear();
            bulletSpawnPos = firePosCannon->GetWorldPos();

            bool flagComplete = false;

            auto PlusXY = [&]()
            {
                if (++curBulletX >= miroWidth)
                {
                    curBulletX = 0;
                    if (++curBulletY >= miroHeight)
                    {
                        curBulletY = 0;
                        flagComplete = true;
                    }
                }
            };

            while (false == flagComplete)
            {
                while (miro[curBulletY][curBulletX] == '0')
                {
                    PlusXY();
                }
                Bullet* elem = new BossBullet;
                elem->Init();
                elem->scalar = 100.0f;
                bullet.emplace_back(elem);
                bullet.back()->Spawn(Vector2(-700.0f + curBulletX * 40.0f,
                    720.0f - curBulletY * 40.0f), Vector2(0.0f, -1.0f));

                PlusXY();
            }

            miroStart = false;
        }
    }

    void Boss::UpdateBrute()
    {
        if (TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::brute]))
        {
            int t = 12;
            while (t--)
            {
                bullet[curBulletIdx++]->Spawn(bulletSpawnPos);
                if (curBulletIdx >= bruteMax) curBulletIdx = 0;
            }

            SOUND->Stop("BulletKingShot");
            SOUND->Play("BulletKingShot");
        }
    }

    void Boss::UpdateShuriken()
    {
        if (TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::shuriken]))
        {
            bullet[curBulletIdx]->Spawn(Vector2(0.0f, 0.0f));
            bullet[curBulletIdx]->col->SetLocalPos(Vector2((curBulletIdx + 1) * 20.0f, (curBulletIdx + 1) * 20.0f));
            curBulletIdx++;
            if (curBulletIdx >= shurikenMax) curBulletIdx = 0;

            SOUND->Stop("BulletKingThrow");
            SOUND->Play("BulletKingThrow");
        }

        int idx = 0;
        for (auto& elem : bullet)
        {
            ShurikenBullet* childElem; 
            if (childElem = dynamic_cast<ShurikenBullet*>(elem))
            {
                if (childElem->ShurikenBulletState == ShurikenBulletState::targeting)
                {
                    childElem->moveDir = targetPos - childElem->Pos();
                }
            }
            idx++;
        }
    }

    void Boss::UpdateGravity()
    {
        if (TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::gravity]))
        {
            bullet[curBulletIdx]->Spawn(bulletSpawnPos, 
                Vector2(RANDOM->Float(-1.0f, 1.0f), 
                    RANDOM->Float(0.0f, 1.0f)));
            curBulletIdx++;
            if (curBulletIdx >= gravityMax) curBulletIdx = 0;

            SOUND->Stop("BulletKingShot");
            SOUND->Play("BulletKingShot");
        }
    }

    void Boss::UpdateWave()
    {
        if (TIMER->GetTick(timeFire, intervalFire[(int)BossPattern::wave]))
        {
            Vector2 fialSpawnPos = bulletSpawnPos;
            bullet[curBulletIdx]->Spawn(fialSpawnPos, Vector2(-0.8f, -1.0f));
            curBulletIdx++;
            if (curBulletIdx >= waveMax) curBulletIdx = 0;

            SOUND->Stop("BulletKingShot");
            SOUND->Play("BulletKingShot");
        }

        int idx = 0;
        for (auto& elem : bullet)
        {
            if (elem->isFired)
            {
                if (elem->increaseState) elem->moveDir.x += 6.0f * DELTA;
                else elem->moveDir.x -= 6.0f * DELTA;

                if (elem->moveDir.x > 0.8f)
                {
                    elem->increaseState = false;
                }
                else if (elem->moveDir.x < -0.8f)
                {
                    elem->increaseState = true;
                }
            }

            idx++;
        }
    }
}