#include "stdafx.h"

namespace Gungeon
{
    Scene03::Scene03()
    {
    }

    Scene03::~Scene03()
    {
    }

    void Scene03::Init()
    {
        // tilemap
        MAPINFO.useGui = false;

        MAPINFO.tilemap = make_shared<ObTileMap>();
        MAPINFO.tilemap->scale = Vector2(100.0f, 100.0f);
        MAPINFO.tilemap->SetWorldPos(Vector2(-app.GetHalfWidth() - 1000.0f, -app.GetHalfHeight() - 1000.0f));
        MAPINFO.imgIdx = 1;
        MAPINFO.tileSize = Vec2i(30, 30);
        MAPINFO.tilemap->ResizeTile(MAPINFO.tileSize);
        MAPINFO.tileColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
        MAPINFO.tileState = 0;
        MAPINFO.tilemap->CreateTileCost();

        InitRoom();

        CAM.position = curRoom->Pos();
        CAM.zoomFactor = Vector3(1.0f, 1.0f, 1.0f);

        player->Spawn(Vector2(0.0f, 0.0f));
        player->state = State::cinematic;

        boss = make_shared<Boss>();
        boss->Spawn(Vector2(0.0f, 300.0f));
        boss->state = State::cinematic;
        boss->idle->isVisible = false;

        cinematic = make_shared<Cinematic>();
        cinematic->cinematicState = CinematicState::cinematicBox1;
        UIOn(false);

        SOUND.Stop("BGM_MapGen");
        SOUND.Stop("BGM_Game");
        SOUND.Play("BGM_Boss");
    }

    void Scene03::InitRoom()
    {
        MAPINFO.imgIdx = 1;

        curRoom = make_shared<Room>();
        curRoom->col->scale = Vector2(1500.0f, 1500.0f);
        curRoom->SetPos(Vector2(0.0f, 0.0f));
        curRoom->col->isVisible = false;

        float tileScale = MAPINFO.tilemap->scale.x;

        Vector2 start;
        Vector2 end;
        Vec2i sour;
        Vec2i dest;

        auto SetTile2 = [&](Vec2i on, int roomIdx)
        {
            MAPINFO.tilemap->SetTile(on,
                Vec2i(RANDOM.Int(floorImgMin.x, floorImgMax.x),
                    RANDOM.Int(floorImgMin.y, floorImgMax.y)),
                MAPINFO.imgIdx,
                (int)TileState::floor,
                Color(0.5f, 0.5f, 0.5f, 1.0f),
                roomIdx);
        };

        auto SetWall2 = [&](Vec2i on, int roomIdx, Vec2i frameIdx)
        {
            MAPINFO.tilemap->SetTile(on,
                frameIdx,
                MAPINFO.imgIdx,
                (int)TileState::wall,
                Color(0.5f, 0.5f, 0.5f, 1.0f),
                roomIdx);
        };

        auto SetTile1 = [&](shared_ptr<Room> elem, int roomIdx)
        {
            shared_ptr<ObRect> r = dynamic_pointer_cast<ObRect>(elem->col);

            start.x = r->lb().x;
            start.y = r->lb().y;
            end.x = r->rt().x;
            end.y = r->rt().y;

            MAPINFO.tilemap->WorldPosToTileIdx(start, sour);
            MAPINFO.tilemap->WorldPosToTileIdx(end, dest);


            for (int y = sour.y + 1; y <= dest.y - 1; y++)
                for (int x = sour.x + 1; x <= dest.x - 1; x++)
                    SetTile2(Vec2i(x, y), roomIdx);

            // wall - top, bottom, left, right, edge

            for (int x = sour.x; x <= dest.x; x++)
            {
                SetWall2(Vec2i(x, dest.y), roomIdx, wallImgDir[DirState::dirT]);
                SetWall2(Vec2i(x, sour.y), roomIdx, wallImgDir[DirState::dirB]);
            }

            for (int y = sour.y; y <= dest.y; y++)
            {
                SetWall2(Vec2i(sour.x, y), roomIdx, wallImgDir[DirState::dirL]);
                SetWall2(Vec2i(dest.x, y), roomIdx, wallImgDir[DirState::dirR]);
            }

            SetWall2(Vec2i(sour.x, sour.y), roomIdx, wallImgDir[DirState::dirLB]);
            SetWall2(Vec2i(sour.x, dest.y), roomIdx, wallImgDir[DirState::dirLT]);
            SetWall2(Vec2i(dest.x, sour.y), roomIdx, wallImgDir[DirState::dirRB]);
            SetWall2(Vec2i(dest.x, dest.y), roomIdx, wallImgDir[DirState::dirRT]);
        };

        SetTile1(curRoom, 99);
    }

    void Scene03::Update()
    {
        if (ImGui::Button("Current Scene"))
            ChangeScene3();

        ChangeUpdateScene();

        if (INPUT.KeyDown('H'))
            ColToggle();

        if (!boss) 
            return;
        
        bool flagCleared = true;

        if (boss->state != State::die)
        {
            flagCleared = false;

            boss->targetPos = player->Pos();
            boss->firePosTargeting->rotation2 = Utility::DirToRadian(player->Pos() - boss->firePosTargeting->GetWorldPos());

            switch (boss->state)
            {
            case State::walk:
                boss->FindPath(MAPINFO.tilemap);
                break;
            case State::attack:
                switch (boss->pattern)
                {
                case BossPattern::tornado:
                case BossPattern::trail:
                    boss->FindPath(MAPINFO.tilemap);
                    break;
                }
            default:
                boss->Stop();
                break;
            }

            switch (boss->pattern)
            {
            case BossPattern::miro:
                if (boss->pushingPlayer)
                {
                    Vector2 dest = Vector2(0.0f, -500.0f);
                    boss->SpawnByForceInMiro(dest);
                    player->col->SetWorldPos(dest);
                    player->Update();
                }
            }
        }

        // 골드 흡수 플래그
        if (flagCleared)
        {
            curRoom->cleared = true;
            boss->dropItem->flagAbsorbed = true;
        }

        if (boss->dropItem->flagAbsorbed)
            boss->dropItem->targetPos = player->Pos();

        CinematicProcess();

        MAPINFO.tilemap->Update();
        player->Update();
        boss->Update();
        cinematic->Update();

        SOUND.SetVolume("BGM_Boss", 0.6f);
    }

    void Scene03::LateUpdate()
    {
        if (!boss) 
            return;

        IntersectPlayer();
        IntersectBoss();
    }

    void Scene03::Render()
    {
        MAPINFO.tilemap->Render();
        if (player) 
            player->shadow->Render();
        if (boss)
        {
            boss->shadow->Render();
            boss->Render();
        }
        if (player) 
            player->Render();

        cinematic->Render();
        if (cinematic->cinematicState == CinematicState::cutScene)
        {
            boss->cutScene->Render();
            DWRITE.RenderText(boss->desc,
                RECT{ 30, 50, (long)app.GetWidth(), (long)app.GetHeight() },
                50.0f,
                L"PF스타더스트");
            DWRITE.RenderText(boss->name,
                RECT{ 60, 110, (long)app.GetWidth(), (long)app.GetHeight() },
                70.0f,
                L"PF스타더스트");
        }
    }

    void Scene03::ResizeScreen()
    {
        if (player) 
            player->ResizeScreen();
        if (boss) 
            boss->ResizeScreen();
        if (cinematic) 
            cinematic->ResizeScreen();
    }

    void Scene03::IntersectPlayer()
    {
        if (MAPINFO.tilemap->isFootOnWall(player->colTile))
            player->StepBack();

        // 플레이어 총알
        for (auto& bulletElem : player->bullet)
        {
            if (bulletElem->isFired)
            {
                if (boss->state != State::die &&
                    bulletElem->col->Intersect(boss->col))
                {
                    Vector2 dir = boss->col->GetWorldPos() - bulletElem->col->GetWorldPos();
                    dir.Normalize();
                    boss->StartHit(bulletElem->damage, dir);
                    bulletElem->Hit(1);
                }

                if (MAPINFO.tilemap->isOnWall(bulletElem->On()))
                    bulletElem->Hit(1);
            }
        }
    }

    void Scene03::IntersectBoss()
    {
        if (player->godMode == false &&
            player->state != State::die &&
            boss->state != State::die &&
            boss->col->Intersect(player->col))
            player->StartHit(1);

        if (MAPINFO.tilemap->isFootOnWall(boss->colTile))
            boss->StepBack();

        // 보스 총알
        for (auto& bulletElem : boss->bullet)
        {
            if (bulletElem->isFired)
            {
                if (player->godMode == false &&
                    player->state != State::die &&
                    boss->state != State::die &&
                    bulletElem->col->Intersect(player->col))
                {
                    player->StartHit(bulletElem->damage);
                    bulletElem->Hit(1);
                }

                Vec2i on = bulletElem->On();
                switch (boss->pattern)
                {
                case BossPattern::trail:
                    if (MAPINFO.tilemap->isOnWall(on))
                    {
                        if (on.y <= 6 || on.y >= 21)
                        {
                            bulletElem->moveDir.y *= -1.0f;
                        }
                        else
                        {
                            bulletElem->moveDir.x *= -1.0f;
                        }
                        bulletElem->col->rotation = Utility::DirToRadian(bulletElem->moveDir);
                        bulletElem->StepBack();
                        bulletElem->Update();
                    }
                    break;
                case BossPattern::gravity:
                    if (MAPINFO.tilemap->isOnWall(on))
                    {
                        if (on.y <= 6 || on.y >= 21)
                        {
                            bulletElem->moveDir.y *= -1.0f;
                            bulletElem->gravity *= -0.8f;
                        }
                        else
                            bulletElem->moveDir.x *= -1.0f;
                        bulletElem->StepBack();
                        bulletElem->Update();
                    }
                    break;
                default:
                    if (MAPINFO.tilemap->isInTileState(bulletElem->Pos(), TileState::wall))
                        bulletElem->Hit(1);
                break;
                }
            }
        }

        if (boss->dropItem->state == State::idle &&
            boss->dropItem->col->Intersect(player->col))
        {
            boss->dropItem->Hit();
            player->PlusMoney(1);
        }

        boss->LateUpdate();
    }

    void Scene03::CinematicProcess()
    {
        Vector2 camVelocity;
        const float camDiff = 30.0f;

        switch (cinematic->cinematicState)
        {
            // 등장연출
        case Gungeon::CinematicState::none:
            if (boss->state == State::die)
                cinematic->cinematicState = CinematicState::cinematicBox3;
            break;

        case Gungeon::CinematicState::cinematicBox1:
            break;

        case Gungeon::CinematicState::cameraTargeting1:
            camVelocity = CAM.position - boss->Pos();
            CAM.position -= camVelocity * DELTA;
            if (abs(CAM.position.x - boss->Pos().x) <= camDiff &&
                abs(CAM.position.y - boss->Pos().y) <= camDiff)
            {
                boss->SpawnAnim();
                cinematic->cinematicState = CinematicState::bossSpawnAnim;
                SOUND.Play("Voice_Boss_Spawn");
            }
            break;

        case Gungeon::CinematicState::bossSpawnAnim:
            if (boss->respawn->frame.x == boss->respawn->maxFrame.x - 1)
            {
                boss->SpawnAnimEnd();
                cinematic->cinematicState = CinematicState::cutScene;
            }
            break;

        case Gungeon::CinematicState::cutScene:
            boss->CutSceneOn();
            break;

        case Gungeon::CinematicState::cameraTargeting2:
            boss->CutSceneOff();

            camVelocity = CAM.position - player->Pos();
            CAM.position -= camVelocity * DELTA;
            if (abs(CAM.position.x - player->Pos().x) <= camDiff &&
                abs(CAM.position.y - player->Pos().y) <= camDiff)
                cinematic->cinematicState = CinematicState::cinematicBox2;
            break;

        case Gungeon::CinematicState::cinematicBox2:
            break;

        case Gungeon::CinematicState::finish:
            boss->StartIdle();
            player->StartIdle();
            UIOn(true);
            cinematic->cinematicState = CinematicState::none;
            break;

            // 사망연출
        case Gungeon::CinematicState::cinematicBox3:
            player->state = State::cinematic;
            UIOn(false);
            break;

        case Gungeon::CinematicState::cameraTargeting3:
            camVelocity = CAM.position - boss->Pos();
            CAM.position -= camVelocity * DELTA;
            if (abs(CAM.position.x - boss->Pos().x) <= camDiff &&
                abs(CAM.position.y - boss->Pos().y) <= camDiff)
                cinematic->cinematicState = CinematicState::bossDieAnim;
            break;

        case Gungeon::CinematicState::bossDieAnim:
            if (boss->realDie)
                cinematic->cinematicState = CinematicState::cameraTargeting4;
            break;

        case Gungeon::CinematicState::cameraTargeting4:
            camVelocity = CAM.position - player->Pos();
            CAM.position -= camVelocity * DELTA;
            if (abs(CAM.position.x - player->Pos().x) <= camDiff &&
                abs(CAM.position.y - player->Pos().y) <= camDiff)
                cinematic->cinematicState = CinematicState::cinematicBox4;
            break;

        case Gungeon::CinematicState::cinematicBox4:
            break;

        case Gungeon::CinematicState::finish2:
            boss->StartDie();
            player->StartIdle();
            UIOn(true);
            cinematic->cinematicState = CinematicState::clear;
            break;

        case Gungeon::CinematicState::clear:
            break;
        }
    }

    void Scene03::UIOn(const bool on)
    {
        player->UIOn(on);
        boss->UIOn(on);
    }

    void Scene03::ChangeUpdateScene()
    {
        if (isChangingScene)
        {
            LIGHT.light.radius -= 2000.0f * DELTA;
            LIGHT.light.lightColor.x += 0.5f * DELTA;
            LIGHT.light.lightColor.y += 0.5f * DELTA;
            LIGHT.light.lightColor.z += 0.5f * DELTA;

            if (TIMER.GetTick(timeFade, 1.0f))
                isChangingScene = false;
        }
        else
        {
            LIGHT.light.radius += 2000.0f * DELTA;
            LIGHT.light.radius = Utility::Saturate(LIGHT.light.radius, 0.0f, 2000.0f);
            LIGHT.light.lightColor.x = 0.5f;
            LIGHT.light.lightColor.y = 0.5f;
            LIGHT.light.lightColor.z = 0.5f;
        }
    }

    void Scene03::ChangeScene3()
    {
        isChangingScene = true;
        SCENE.ChangeScene("Scene03", 1.0f);
    }

    // 치트
    void Scene03::ColToggle()
    {
        player->ColToggle();
        boss->ColToggle();
    }
}