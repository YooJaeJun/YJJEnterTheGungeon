#include "stdafx.h"

namespace Gungeon
{
    Scene02::Scene02()
    {
        for (auto& elem : enemy)
        {
            elem = nullptr;
        }
    }

    Scene02::~Scene02()
    {
    }

    void Scene02::Init()
    {
        MAP->useGui = false;

        spawnEffect.resize(enemyMax);
        for (auto& elem : spawnEffect)
        {
            if (!elem)
            {
                elem = new Effect;
                elem->idle = new ObImage(L"EnterTheGungeon/Level/Spawn.png");
                elem->state = State::die;
                elem->idle->maxFrame.x = 18;
                elem->idle->scale = Vector2(663.0f / 18.0f, 39.0f) * 3.5f;
                elem->idle->isVisible = false;
                elem->intervalDie = 1.8f;
            }
        }

        SpawnEnemy();

        gate = new Gate;

        door.resize(doorMax);
        for (auto& elem : door)
        {
            elem = new Door;
            elem->col->scale = Vector2(16.0f, 128.0f / 8.0f) * 6.0f;
            elem->col->pivot = OFFSET_LB;
            elem->SetPos(DEFAULTSPAWN);
            elem->idle = new ObImage(L"EnterTheGungeon/Level/Door.png");
            elem->idle->maxFrame.y = 8;
            elem->idle->scale = Vector2(24.0f, 192.0f / 8.0f) * 4.0f;
            elem->idle->SetParentRT(*elem->col);
            elem->idle->pivot = OFFSET_LB;
        }

        int idx = 0;
        if (mapGen && mapGen->selectedRooms.size() > 0)
        {
            for (auto& elem : mapGen->selectedRooms)
            {
                if (idx == 1)
                {
                    elem->treasureSpawner->isVisible = false;
                }
                for (auto& enemySpawerElem : elem->enemySpawner)
                {
                    enemySpawerElem->isVisible = false;
                }
                for (auto& gateSpawerElem : elem->gateSpawner)
                {
                    gateSpawerElem->isVisible = false;
                }
                idx++;
            }
        }

        cinematic = new Cinematic;

        SOUND->Stop("BGM_MapGen");
        SOUND->Play("BGM_Game");
    }

    void Scene02::Release()
    {
        int idx = 0;
        for (auto& elem : mapGen->selectedRooms)
        {
            if (idx != 0 and idx != 1)
            {
                elem->cleared = false;
            }
            idx++;
        }
        for (auto& elem : enemy) SafeRelease(elem);
        SafeRelease(curRoom);
        SafeRelease(gate);
        SafeRelease(cinematic);
        for (auto& elem : door) SafeRelease(elem);
        SafeRelease(treasureBox);
    }

    void Scene02::Update()
    {
        if (ImGui::Button("Prev Scene"))
        {
            ChangeScene1();
        }
        ImGui::SameLine();
        if (ImGui::Button("Current Scene"))
        {
            ChangeScene2();
        }
        ImGui::SameLine();
        if (ImGui::Button("Next Scene"))
        {
            ChangeScene3();
        }

        ChangeUpdateScene();

        if (INPUT->KeyDown('H'))
        {
            ColToggle();
        }

        switch (gameState)
        {
        case Gungeon::GameState::start:
            Start();
            break;
        case Gungeon::GameState::waitingRoom:
            WaitingRoom();
            break;
        case Gungeon::GameState::enteringRoom:
            EnteringRoom();
            break;
        case Gungeon::GameState::waitingSpawn:
            WaitingSpawn();
            break;
        case Gungeon::GameState::fight:
            Fight();
            break;
        }

        GateProcess();

        if (mapGen) mapGen->Update();
        gate->Update();
        if (treasureBox) treasureBox->Update();
        for (auto& elem : door) elem->Update();
        for (auto& elem : spawnEffect) if (elem) elem->Update();
        player->Update();
        for (auto& elem : enemy) if (elem) elem->Update();
        cinematic->Update();

        SOUND->SetVolume("BGM_Game", 0.6f);
    }

    void Scene02::LateUpdate()
    {
        if (mapGen)
        {
            mapGen->LateUpdate();
        }

        switch (gameState)
        {
        case Gungeon::GameState::start:
        case Gungeon::GameState::waitingRoom:
            break;
        case Gungeon::GameState::enteringRoom:
        case Gungeon::GameState::waitingSpawn:
        case Gungeon::GameState::fight:
            IntersectPlayer();
            IntersectEnemy();
            break;
        }
    }

    void Scene02::Render()
    {
        if (mapGen)
        {
            mapGen->Render();

            for (auto& elem : spawnEffect) if (elem) elem->Render();
        }

        gate->Render();
        for (auto& elem : door) elem->Render();
        if (treasureBox) treasureBox->Render();

        if (player) player->shadow->Render();
        for (auto& elem : enemy) if (elem) elem->shadow->Render();
        for (auto& elem : enemy) if (elem) elem->Render();
        if (player) player->Render();
        cinematic->Render();
    }

    void Scene02::ResizeScreen()
    {
        mapGen->ResizeScreen();

        player->ResizeScreen();

        gate->ResizeScreen();

        cinematic->ResizeScreen();
    }

    void Scene02::Start()
    {
        if (!mapGen) return;

        mapGen->Update();

        if (mapGen->selectedRooms.size() > 0)
        {
            gameState = GameState::waitingRoom;
        }
    }

    void Scene02::WaitingRoom()
    {
        curRoomIdx = 0;
        curRoom = mapGen->selectedRooms[curRoomIdx];
        SetCamera();
        SpawnPlayer();

        gameState = GameState::enteringRoom;
    }

    void Scene02::EnteringRoom()
    {
        // 골드 흡수
        for (auto& elem : enemy)
        {
            if (elem->dropItem->flagAbsorbed)
            {
                elem->dropItem->targetPos = player->Pos();
            }
        }

        // 다음 방 입장 판단
        Int2 playerOn;
        MAP->tilemap->WorldPosToTileIdx(player->Pos(), playerOn);
        TileState tileState = MAP->tilemap->GetTileState(playerOn);

        if (tileState == TileState::floor)
        {
            afterRoomIdx = MAP->tilemap->Tiles[playerOn.x][playerOn.y].roomIdx;

            if (afterRoomIdx > 0 &&
                curRoomIdx != afterRoomIdx)
            {
                curRoomIdx = afterRoomIdx;
                curRoom = mapGen->selectedRooms[curRoomIdx];


                switch (curRoom->roomType)
                {
                case RoomType::treasure:
                    SpawnTreasureBox();
                    gameState = GameState::fight;
                    break;

                case RoomType::enemy:
                {
                    if (false == mapGen->selectedRooms[afterRoomIdx]->cleared)
                    {
                        SpawnEffect();
                        SOUND->Play("EnemySpawning");

                        int idx = 0;
                        for (auto& on : curRoom->doorTileIdxs)
                        {
                            MAP->tilemap->SetTileState(on, TileState::wall);
                            door[idx]->Spawn(MAP->tilemap->TileIdxToWorldPos(on), MAP->tilemap->GetTileDir(on));
                            idx++;
                        }

                        gameState = GameState::waitingSpawn;
                    }
                    break;
                }

                default:
                    break;
                }
            }
        }
    }

    void Scene02::WaitingSpawn()
    {
        bool flagSpawnEffectAllDie = true;

        for (auto& elem : spawnEffect)
        {
            if (elem->state != State::die)
            {
                flagSpawnEffectAllDie = false;
            }
        }

        if (flagSpawnEffectAllDie)
        {
            SpawnEnemy();

            gameState = GameState::fight;

            SOUND->Play("EnemySpawn");
        }
    }

    void Scene02::Fight()
    {
        bool flagAllDie = true;

        for (auto& elem : enemy)
        {
            if (elem->state != State::die)
            {
                flagAllDie = false;
                elem->targetPos = player->Pos();
            }

            switch (elem->state)
            {
            case State::attack:
            case State::cinematic:
            case State::die:
                break;
            default:
                elem->FindPath(MAP->tilemap);
            }
        }

        if (flagAllDie)
        {
            curRoom->cleared = true;
        }

        if (curRoom->cleared)
        {
            switch (curRoom->roomType)
            {
            case RoomType::treasure:
                break;

            default:
                int idx = 0;
                for (auto& elem : curRoom->doorTileIdxs)
                {
                    MAP->tilemap->SetTileState(elem, TileState::door);

                    door[idx]->Disappear();

                    idx++;
                }

                for (auto& elem : enemy)
                {
                    if (elem)
                    {
                        elem->dropItem->flagAbsorbed = true;
                    }
                }
                break;
            }

            roomClearCount++;

            gameState = GameState::enteringRoom;
        }
    }

    void Scene02::SetCamera()
    {
        if (curRoom && curRoom->col) CAM->position = curRoom->Pos();
        CAM->zoomFactor = Vector3(1.0f, 1.0f, 1.0f);
    }

    void Scene02::SpawnPlayer()
    {
        player->Spawn(curRoom->Pos());
    }

    void Scene02::SpawnEffect()
    {
        int idx = 0;
        for (auto& elem : spawnEffect)
        {
            elem->Spawn(curRoom->enemySpawner[idx]->GetWorldPos());
            idx++;
        }
    }

    void Scene02::SpawnEnemy()
    {
        int idx = 0;
        for (auto& elem : enemy)
        {
            SafeRelease(elem);
            int randEnemyClass = RANDOM->Int(0, 6);
            if (randEnemyClass < 2) elem = new Enemy3();
            else if (randEnemyClass < 4) elem = new Enemy1();
            else elem = new Enemy2();
            if (curRoom) elem->Spawn(curRoom->enemySpawner[idx]->GetWorldPos());
            idx++;
        }
    }

    void Scene02::SpawnTreasureBox()
    {
        if (!treasureBox)
        {
            treasureBox = new TreasureBox;
            treasureBox->Spawn(curRoom->treasureSpawner->GetWorldPos());
        }
    }

    void Scene02::IntersectPlayer()
    {
        if (player->state != State::die)
        {
            if (MAP->tilemap->isFootOnWall(player->colTile))
            {
                player->StepBack();
            }

            if (MAP->tilemap->isBodyOnPit(player->colTile) && 
                player->state != State::fall &&
                player->state != State::respawn &&
                player->state != State::roll)
            {
                player->StartFall();
            }

            if (false == gate->flagIntersectPlayer &&
                player->col->Intersect(gate->col))
            {
                player->col->MoveWorldPos(Vector2(150.0f, -100.0f) * DELTA);
            }

            if (treasureBox)
            {
                if (treasureBox->treasureState == TreasureState::spawn)
                {
                    if (player->col->Intersect(treasureBox->col))
                    {
                        player->flagInteractionUI = true;

                        if (INPUT->KeyDown('E'))
                        {
                            treasureBox->treasureState = TreasureState::opening;
                            player->flagInteractionUI = false;
                            SOUND->Play("ChestOpen");
                        }
                    }
                }
                else
                {
                    bool flagOnceInteraction = false;
                    int weaponIdx = 0;
                    for (auto& elem : treasureBox->weapon)
                    {
                        if (elem &&
                            elem->state == State::idle &&
                            player->col->Intersect(elem->col))
                        {
                            if (INPUT->KeyDown('E'))
                            {
                                elem->Hit();
                                player->EquipWeapon(elem);
                            }
                            else
                            {
                                flagOnceInteraction = true;
                            }
                        }
                        weaponIdx++;
                    }

                    if (flagOnceInteraction)
                    {
                        player->flagInteractionUI = true;
                    }
                    else
                    {
                        player->flagInteractionUI = false;
                    }
                }
            }


            if (curRoom)
            {
                for (auto& elem : door)
                {
                    if (player->colTile->Intersect(elem->col))
                    {
                        DirState doorDir = MAP->tilemap->GetTileDir(elem->On());
                        int x, y;
                        switch (doorDir)
                        {
                        case dirB:
                            x = -dx[DirState::dirB];
                            y = -dy[DirState::dirB];
                            break;
                        case dirLB:
                            x = -dx[DirState::dirLB];
                            y = -dy[DirState::dirLB];
                            break;
                        case dirRB:
                            x = -dx[DirState::dirRB];
                            y = -dy[DirState::dirRB];
                            break;
                        case dirT:
                            x = -dx[DirState::dirT];
                            y = -dy[DirState::dirT];
                            break;
                        case dirLT:
                            x = -dx[DirState::dirLT];
                            y = -dy[DirState::dirLT];
                            break;
                        case dirRT:
                            x = -dx[DirState::dirRT];
                            y = -dy[DirState::dirRT];
                            break;
                        case dirL:
                            x = -dx[DirState::dirL];
                            y = -dy[DirState::dirL];
                            break;
                        case dirR:
                            x = -dx[DirState::dirR];
                            y = -dy[DirState::dirR];
                            break;
                        case dirNone:
                            x = -dx[DirState::dirNone];
                            y = -dy[DirState::dirNone];
                            break;
                        }
                        player->col->MoveWorldPos(Vector2(static_cast<float>(x), static_cast<float>(y)));
                    }
                }
            }
        }

        // 플레이어 총알
        for (auto& bulletElem : player->bullet)
        {
            if (bulletElem->isFired)
            {
                for (auto& enemyElem : enemy)
                {
                    if (enemyElem->state != State::die &&
                        bulletElem->col->Intersect(enemyElem->col))
                    {
                        Vector2 dir = enemyElem->col->GetWorldPos() - bulletElem->col->GetWorldPos();
                        dir.Normalize();
                        enemyElem->StartHit(bulletElem->damage, dir);
                        bulletElem->Hit(1);
                    }
                }

                if (MAP->tilemap->isOnWall(bulletElem->On()))
                {
                    bulletElem->Hit(1);
                }
            }
        }
    }

    void Scene02::IntersectEnemy()
    {
        for (auto& enemyElem : enemy)
        {
            if (player->godMode == false &&
                player->state != State::die && 
                enemyElem->state != State::die &&
                enemyElem->col->Intersect(player->col))
            {
                player->StartHit(1);
            }

            if (MAP->tilemap->isFootOnWall(enemyElem->colTile))
            {
                enemyElem->StepBack();
            }

            if (enemyElem->col->Intersect(gate->col))
            {
                Vector2 velocity = gate->Pos() - enemyElem->Pos();
                enemyElem->col->MoveWorldPos(-velocity * DELTA);
            }

            // 적 총알
            for (auto& bulletElem : enemyElem->bullet)
            {
                if (bulletElem->isFired)
                {
                    if (player->godMode == false &&
                        player->state != State::die &&
                        enemyElem->state != State::die &&
                        bulletElem->col->Intersect(player->col))
                    {
                        player->StartHit(bulletElem->damage);
                        bulletElem->Hit(1);
                    }

                    if (MAP->tilemap->isOnWall(bulletElem->On()))
                    {
                        bulletElem->Hit(1);
                    }
                }
            }

            if (enemyElem->dropItem->state == State::idle &&
                enemyElem->dropItem->col->Intersect(player->col))
            {
                enemyElem->dropItem->Hit();
                player->PlusMoney(1);
            }

            enemyElem->LateUpdate();
        }
    }

    void Scene02::GateProcess()
    {
        if (roomClearCount >= roomClearCountForBossBattle &&
            curRoom->roomType != RoomType::start &&
            curRoom->roomType != RoomType::treasure &&
            curRoom->cleared)
        {
            roomClearCount = 0;
            gate->Spawn(curRoom->Pos());

        }

        switch (gate->gateState)
        {
        case Gungeon::GateState::none:
        case Gungeon::GateState::opening:
            break;

        case Gungeon::GateState::open:
            if (gate->colTile->Intersect(player->col))
            {
                gate->flagIntersectPlayer = true;
            }
            break;

        case Gungeon::GateState::cinematic:
            cinematic->box[0]->img->isVisible = true;
            cinematic->box[1]->img->isVisible = true;
            cinematic->BoxUp(true);
            player->state = State::cinematic;
            player->UIOn(true);
            break;

        case Gungeon::GateState::process:
            gate->playerDest = Vector2(gate->col->GetWorldPos().x, gate->col->GetWorldPos().y + 70.0f);

            if (abs(gate->playerDest.x - player->Pos().x) > 1.0f &&
                abs(gate->playerDest.y - player->Pos().y) > 1.0f)
            {
                player->idle->isVisible = false;
                player->roll->isVisible = false;
                player->weapons[player->curWeaponIdx]->idle->isVisible = false;
                player->weapons[player->curWeaponIdx]->imgReloading->isVisible = false;
                player->walk->isVisible = true;
                Vector2 dir = gate->playerDest - player->Pos();
                dir.Normalize();
                player->col->MoveWorldPos(dir * 80.0f * DELTA);
            }
            else
            {
                gate->gateState = GateState::setting;
            }
            break;

        case Gungeon::GateState::setting:
            player->walk->isVisible = false;
            player->idle->isVisible = true;
            break;

        case Gungeon::GateState::set:
            break;

        case Gungeon::GateState::closing:
            if (gate->flagPlayerDisappear)
            {
                player->idle->isVisible = false;
                player->walk->isVisible = false;
                player->weapons[player->curWeaponIdx]->idle->isVisible = false;
                player->shadow->isVisible = false;
            }
            break;

        case Gungeon::GateState::closed:
            gate->gateState = GateState::none;
            ChangeScene3();
            break;
        }
    }

    void Scene02::ChangeUpdateScene()
    {
        if (isChangingScene)
        {
            LIGHT->light.radius -= 2000.0f * DELTA;
            LIGHT->light.lightColor.x += 0.5f * DELTA;
            LIGHT->light.lightColor.y += 0.5f * DELTA;
            LIGHT->light.lightColor.z += 0.5f * DELTA;

            if (TIMER->GetTick(timeFade, 1.0f))
            {
                isChangingScene = false;
            }
        }
        else
        {
            LIGHT->light.radius += 2000.0f * DELTA;
            LIGHT->light.radius = Utility::Saturate(LIGHT->light.radius, 0.0f, 2000.0f);
            LIGHT->light.lightColor.x = 0.5f;
            LIGHT->light.lightColor.y = 0.5f;
            LIGHT->light.lightColor.z = 0.5f;
        }
    }

    void Scene02::ChangeScene1()
    {
        gameState = GameState::start;
        isChangingScene = true;
        SCENE->ChangeScene("Scene01", 1.0f);
    }

    void Scene02::ChangeScene2()
    {
        gameState = GameState::start;
        if (curRoom)
        {
            for (auto& elem : curRoom->doorTileIdxs)
            {
                MAP->tilemap->SetTileState(elem, TileState::door);
            }
        }
        Release();
        Init();
    }

    void Scene02::ChangeScene3()
    {
        isChangingScene = true;
        {
            Scene03* tempScene = new Scene03();
            tempScene->player = player;
            SCENE->AddScene("Scene03", tempScene);
        }
        SCENE->ChangeScene("Scene03", 1.0f);
    }

    // 치트
    void Scene02::ColToggle()
    {
        player->ColToggle();

        for (auto& elem : enemy)
        {
            elem->ColToggle();
        }

        for (auto& elem : door) elem->ColToggle();
        gate->ColToggle();
    }
}
