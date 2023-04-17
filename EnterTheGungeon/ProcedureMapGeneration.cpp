#include "stdafx.h"

namespace Gungeon
{
    ProcedureMapGeneration::ProcedureMapGeneration()
    {
        Init();
    }

    void ProcedureMapGeneration::Init()
    {
        Clear();
        MAPINFO.useGui = false;
        LIGHT.light.radius = 4000.0f;
        state = MapGenState::none;
        finalState = state;
    }

    void ProcedureMapGeneration::Clear()
    {
        candidateRooms.clear();
        selectedRooms.clear();
        roomNode.clear();
        triangulation.nodesLinked.clear();
        triangulation.edges.clear();
        triangulation.triangles.clear();
        triangulation.nodesForIndex.clear();
        linesTriangulated.clear();
        linesMST.clear();
        passages.clear();
    }

    void ProcedureMapGeneration::Update()
    {
        if (state != MapGenState::finish)
        {
            if (ImGui::Button("Next Step"))
                state = (MapGenState)((int)finalState + 1);
            ImGui::SameLine();
            if (ImGui::Button("Auto"))
            {
                Init();
                autoProcess = true;
            }
        }

        switch (state)
        {
        case MapGenState::spray:
            TIMER.CheckStartTimer();
            Spray();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::spread:
            TIMER.CheckStartTimer();
            Spread();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::select:
            TIMER.CheckStartTimer();
            Select();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::triangulate:
            TIMER.CheckStartTimer();
            Triangulate();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::span:
            TIMER.CheckStartTimer();
            Spanning();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::loop:
            TIMER.CheckStartTimer();
            Loop();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::clean:
            TIMER.CheckStartTimer();
            Clean();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::roomTile:
            TIMER.CheckStartTimer();
            RoomTile();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::passageTile:
            TIMER.CheckStartTimer();
            PassageTile();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::passagePitTile:
            TIMER.CheckStartTimer();
            PassagePitTile();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::prop:
            TIMER.CheckStartTimer();
            Prop();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::spawner:
            TIMER.CheckStartTimer();
            Spawner();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::propPit:
            TIMER.CheckStartTimer();
            PropPit();
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::propWall:
            TIMER.CheckStartTimer();
            PropWall();
            MAPINFO.useGui = true;
            TIMER.DebugPrintTimer(debugStateText[(int)state].c_str());
            break;

        case MapGenState::finish:
            Finish();
            autoProcess = false;
            break;
        }//switch

        if (state != MapGenState::finish)
        {
            if (autoProcess)
            {
                finalState = state;
                state = (MapGenState)((int)state + 1);
            }
            else if (state != MapGenState::none)
            {
                finalState = state;
                state = MapGenState::none;
            }
        }


        for (auto& elem : candidateRooms) 
            if (elem) 
                elem->Update();
        for (auto& elem : selectedRooms) 
            if (elem) 
                elem->Update();
        for (auto& elem : linesTriangulated) 
            elem.Update();
        for (auto& elem : linesMST) 
            elem.Update();
    }

    void ProcedureMapGeneration::LateUpdate()
    {
    }

    void ProcedureMapGeneration::Render()
    {
        for (auto& elem : candidateRooms) 
            if (elem) 
                elem->Render();
        for (auto& elem : selectedRooms) 
            if (elem) 
                elem->Render();
        for (auto& elem : linesTriangulated) 
            elem.Render();
        for (auto& elem : linesMST) 
            elem.Render();
    }

    void ProcedureMapGeneration::ResizeScreen()
    {
    }

    void ProcedureMapGeneration::Spray()
    {       
        // room
        roomScaleForSelect = 1100.0f;
        candidateRooms = vector<shared_ptr<Room>>(roomMax);
        int idx = 0;
        for (auto& room : candidateRooms)
        {
            room = make_shared<Room>();

            // 일부 방 선택할 만한 크기 보장
            if (idx < 5)
            {
                room->col->scale.x = 100.0f * RANDOM.Int(13, 15);
                room->col->scale.y = 100.0f * RANDOM.Int(13, 15);
            }
            else
            {
                room->col->scale.x = 100.0f * RANDOM.Int(9, 14);
                room->col->scale.y = 100.0f * RANDOM.Int(9, 14);
            }

            map<Vec2i, int> dic;
            int xRand = 0, yRand = 0;
            do {
                xRand = RANDOM.Int(-10, 10);
                yRand = RANDOM.Int(-10, 10);
            } while (dic[Vec2i(xRand, yRand)]);

            dic[Vec2i(xRand, yRand)]++;

            room->SetPosX(100.0f * xRand);
            room->SetPosY(100.0f * yRand);
            room->col->isFilled = false;
            room->col->color = Color(1.0f, 0.6f, 0.6f);
            room->col->collider = Collider::rect;

            idx++;
        }
    }

    void ProcedureMapGeneration::Spread()
    {
        while (1)
        {
            bool flagSpread = true;

            for (auto& room : candidateRooms)
            {
                for (auto& room2 : candidateRooms)
                {
                    if (room == room2) 
                        continue;

                    if (room->col->Intersect(room2->col))
                    {
                        Vector2 dir = room->col->GetWorldPos() - room2->col->GetWorldPos();

                        float x = 0.0f, y = 0.0f;
                        if (dir.x >= 0.0f) 
                            x = MAPINFO.tilemap->scale.x / 2.0f;
                        else 
                            x = MAPINFO.tilemap->scale.x * -1.0f / 2.0f;

                        if (dir.y >= 0.0f) 
                            y = MAPINFO.tilemap->scale.y / 2.0f;
                        else 
                            y = MAPINFO.tilemap->scale.y * -1.0f / 2.0f;

                        room->SetPos(Vector2(room->Pos().x + x, room->Pos().y + y));
                        flagSpread = false;
                    }
                }
                room->Update();
            }

            if (flagSpread)
                break;
        }
    }

    void ProcedureMapGeneration::Select()
    {
        for (auto& elem : candidateRooms)
        {
            if (elem->col->scale.x > roomScaleForSelect &&
                elem->col->scale.y > roomScaleForSelect)
            {
                elem->col->scale.x -= 200.0f;
                elem->col->scale.y -= 200.0f;
                Vec2i on;
                if (MAPINFO.tilemap->WorldPosToTileIdx(elem->Pos(), on))
                    elem->SetPos(MAPINFO.tilemap->TileIdxToWorldPos(on));
                elem->col->isFilled = true;
                elem->selected = true;

                selectedRooms.emplace_back(elem);
            }
        }
    }

    void ProcedureMapGeneration::Triangulate()
    {
        int idx = 0;
        for (auto& elem : selectedRooms)
        {
            // 들로네 삼각분할 위한 노드
            roomNode.emplace_back(Vec2f(idx, elem->Pos()));
            idx++;
        }

        triangulation.Triangulate(roomNode);
        linesTriangulated = triangulation.edges;

        for (auto& elem : linesTriangulated)
            elem.color = Color(1.0f, 1.0f, 1.0f);
    }

    void ProcedureMapGeneration::Spanning()
    {
        MSTPRIM.FindPath(triangulation, linesTriangulated, linesMST);
    }

    void ProcedureMapGeneration::Loop()
    {
        size_t count = max(1, linesTriangulated.size() / 8);

        while (count--)
        {
            int rand = static_cast<int>(RANDOM.Int(0, linesTriangulated.size() - 1));
            bool flag = false;
            for (auto& elem : linesMST)
            {
                if ((elem.V() == linesTriangulated[rand].V() && elem.W() == linesTriangulated[rand].W()) ||
                    (elem.V() == linesTriangulated[rand].W() && elem.W() == linesTriangulated[rand].V()))
                {
                    count++;
                    flag = true;
                    break;
                }
            }
            if (false == flag)
            {
                // 노드의 인덱스를 검사
                linesTriangulated[rand].SetVIdx(triangulation.nodesForIndex[linesTriangulated[rand].V()]);
                linesTriangulated[rand].SetWIdx(triangulation.nodesForIndex[linesTriangulated[rand].W()]);

                linesTriangulated[rand].color = Color(0.5f, 0.5f, 1.0f);
                linesMST.emplace_back(linesTriangulated[rand]);
            }
        }
    }

    void ProcedureMapGeneration::Clean()
    {
        linesTriangulated.clear();
        for (auto& elem : candidateRooms)
            if (false == elem->selected)
                elem.reset();
        candidateRooms.clear();
    }

    void ProcedureMapGeneration::RoomTile()
    {
        Vec2i sour, dest;
        int roomIdx = 0;
        for (auto& elem : selectedRooms)
        {
            sour = elem->TileLB();
            dest = elem->TileRT();

            SetTileRange(TileState::floor, floorImgMin, floorImgMax, sour, dest, roomIdx);
            SetTileAllDir(TileState::wall, wallImgDir, sour, dest, roomIdx);

            elem->col->isVisible = false;
            roomIdx++;
        }

        selectedRooms[(int)RoomType::start]->roomType = RoomType::start;
        selectedRooms[(int)RoomType::start]->cleared = true;
    }

    void ProcedureMapGeneration::PassageTile()
    {
        // room index와 node index 연결정보
        for (auto& elem : linesMST)
        {
            for (int roomIdx = 0; roomIdx < selectedRooms.size(); roomIdx++)
            {
                Vector2 v = Vector2(elem.V().x, elem.V().y);
                if (NearlyEqualVector2(v, selectedRooms[roomIdx]->Pos()))
                    nodesForRoomIndex[v] = roomIdx;

                Vector2 w = Vector2(elem.W().x, elem.W().y);
                if (NearlyEqualVector2(w, selectedRooms[roomIdx]->Pos()))
                    nodesForRoomIndex[w] = roomIdx;
            }
        }

        // 1. 8방향 체크해 문의 방향을 정하고, 2. 방향에 따라 다른 이미지, 3. 모서리에 위치할 시 예외처리
        auto SetDoor = [&](Vec2i on, const int roomIdx, const DirState dir)
        {
            deque<bool> dirWall(8);

            for (int i = 0; i < 8; i++)
            {
                int nx = on.x + dx[i];
                int ny = on.y + dy[i];

                if (MAPINFO.tilemap->GetTileState(Vec2i(nx, ny)) == TileState::wall ||
                    MAPINFO.tilemap->GetTileState(Vec2i(nx, ny)) == TileState::door)
                    dirWall[i] = true;
            }

            Vec2i doorTileIdx = on;
            Vec2i floorTileIdx;

            // 가로, 세로
            if (dirWall[DirState::dirB] && dirWall[DirState::dirT]) 
            {}
            else if (dirWall[DirState::dirL] && dirWall[DirState::dirR]) 
            {}
            // 모서리에 위치 시 옆에 설치 예외처리
            else
            {
                floorTileIdx.x = on.x;
                floorTileIdx.y = on.y;

                if (dirWall[DirState::dirL] && dirWall[DirState::dirB])
                {
                    doorTileIdx.x = on.x + dx[DirState::dirL];
                    doorTileIdx.y = on.y + dy[DirState::dirL];
                }
                else if (dirWall[DirState::dirB] && dirWall[DirState::dirR])
                {
                    doorTileIdx.x = on.x + dx[DirState::dirR];
                    doorTileIdx.y = on.y + dy[DirState::dirR];
                }
                else if (dirWall[DirState::dirR] && dirWall[DirState::dirT])
                {
                    doorTileIdx.x = on.x + dx[DirState::dirR];
                    doorTileIdx.y = on.y + dy[DirState::dirR];
                }
                else if (dirWall[DirState::dirT] && dirWall[DirState::dirL])
                {
                    doorTileIdx.x = on.x + dx[DirState::dirL];
                    doorTileIdx.y = on.y + dy[DirState::dirL];
                }

                MAPINFO.tilemap->SetTile(floorTileIdx,
                    Vec2i(RANDOM.Int(floorImgMin.x, floorImgMax.x),
                        RANDOM.Int(floorImgMin.y, floorImgMax.y)),
                    MAPINFO.imgIdx,
                    (int)TileState::floor, 
                    Color(0.5f, 0.5f, 0.5f),
                    -1, 
                    dir);
            }

            MAPINFO.tilemap->SetTile(doorTileIdx,
                Vec2i(RANDOM.Int(floorImgMin.x, floorImgMax.x),
                    RANDOM.Int(floorImgMin.y, floorImgMax.y)),
                MAPINFO.imgIdx,
                (int)TileState::door,
                Color(0.5f, 0.5f, 0.5f),
                -1, 
                dir);

            selectedRooms[roomIdx]->doorTileIdxs.emplace_back(doorTileIdx);
        };


        // 복도 A* 2회 - 방 간 최단거리 위함
        // 1. 시작방 중점 -> 시작방 벽
        // 2. 시작방 벽 -> 끝방 벽

        auto FindValidLoad = [&](Vec2i sour, Vec2i dest, bool first, const int passageIdx) ->Vec2i
        {
            if (ASTAR.FindPath(MAPINFO.tilemap, sour, dest, way, false))
            {
                int sourRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(sour);
                int destRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(dest);

                Vec2i beforeOn = way.back().idx;
                int beforeRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(beforeOn);
                DirState beforeDir = MAPINFO.tilemap->GetTileDir(beforeOn);

                for (int cur = way.size() - 2; cur >= 0; cur--)
                {
                    Vec2i curOn = way[cur].idx;
                    TileState curTileState = MAPINFO.tilemap->GetTileState(curOn);
                    int curRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(curOn);
                    DirState curDir = MAPINFO.tilemap->GetTileDir(curOn);

                    if (curTileState == TileState::none)
                    {
                        MAPINFO.tilemap->SetTile(curOn,
                            Vec2i(RANDOM.Int(floorImgMin.x, floorImgMax.x),
                                RANDOM.Int(floorImgMin.y, floorImgMax.y)),
                            MAPINFO.imgIdx,
                            (int)TileState::floor);

                        passages[passageIdx].emplace_back(curOn);
                    }

                    if (first && curRoomIdx != beforeRoomIdx)
                    {
                        SetDoor(beforeOn, beforeRoomIdx, beforeDir);
                        way.clear();
                        return curOn;
                    }
                    else if (!first && curRoomIdx == destRoomIdx)
                    {
                        SetDoor(curOn, curRoomIdx, curDir);
                        way.clear();
                        return curOn;
                    }
                    else if (!first &&
                        curRoomIdx != destRoomIdx &&
                        curTileState == TileState::wall)
                    {
                        SetDoor(curOn, curRoomIdx, curDir);
                    }

                    beforeOn = way[cur].idx;
                    beforeRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(beforeOn);
                    beforeDir = MAPINFO.tilemap->GetTileDir(beforeOn);
                }
            }

            // 위에서 길찾아 리턴되었어야 함
            assert(false);
        };

        // 시작
        int idx = 0;
        passages.resize(linesMST.size());
        for (auto& elem : linesMST)
        {
            const Vec2f& v = elem.V();
            const Vec2f& w = elem.W();

            Vec2f mid = Vec2f((w.x + v.x) / 2.0f, (w.y + v.y) / 2.0f);

            // Vector2 vScaleHalf = selectedRooms[nodesForRoomIndex[v]]->col->scale / 2.0f;
            // Vector2 wScaleHalf = selectedRooms[nodesForRoomIndex[w]]->col->scale / 2.0f;

            Vector2	start = Vector2(v.x, v.y);
            Vector2 end = Vector2(w.x, w.y);

            Vec2i sour, dest;
            MAPINFO.tilemap->WorldPosToTileIdx(start, sour);
            MAPINFO.tilemap->WorldPosToTileIdx(end, dest);

            sour = FindValidLoad(sour, dest, true, idx);
            FindValidLoad(sour, dest, false, idx);

            idx++;
        }

        linesMST.clear();
    }

    void ProcedureMapGeneration::PassagePitTile()
    {
        for (auto& elem : passages)
        {
            for (auto& on : elem)
            {
                for (int j = 0; j < 8; j++)
                {
                    int nx = on.x + dx[j];
                    int ny = on.y + dy[j];
                    if (nx < 0 || nx >= MAPINFO.tileSize.x || ny < 0 || ny >= MAPINFO.tileSize.y) continue;

                    if (MAPINFO.tilemap->GetTileState(Vec2i(nx, ny)) == TileState::none)
                    {
                        MAPINFO.tilemap->SetTile(Vec2i(nx, ny),
                            Vec2i(RANDOM.Int(passagePitImgMin.x, passagePitImgMax.x),
                                RANDOM.Int(passagePitImgMin.y, passagePitImgMax.y)),
                            MAPINFO.imgIdx,
                            (int)TileState::pit);
                    }
                }
            }
        }
    }

    void ProcedureMapGeneration::Prop()
    {
        for (auto& elem : selectedRooms)
        {
            Vec2i on;
            MAPINFO.tilemap->WorldPosToTileIdx(elem->Pos(), on);
            Vec2i scaleOn = Vec2i(static_cast<int>(elem->col->scale.x / 100), static_cast<int>(elem->col->scale.y / 100));
            Vec2i halfScaleOn = scaleOn / 2;

            int xStart = elem->TileLB().x + 1;
            int xEnd = elem->TileRB().x - 1;
            int yStart = elem->TileLB().y + 1;
            int yEnd = elem->TileLT().y - 1;

            int tileRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(on);
            int t = 3;
            while (t--)
            {
                int rx = RANDOM.Int(xStart, xEnd);
                int ry = RANDOM.Int(yStart, yEnd);

                if (MAPINFO.tilemap->GetTileState(Vec2i(rx, ry)) == TileState::floor)
                {
                    MAPINFO.tilemap->SetTile(Vec2i(rx, ry),
                        Vec2i(RANDOM.Int(propImgMin.x, propImgMax.x),
                            RANDOM.Int(propImgMin.y, propImgMax.y)),
                        MAPINFO.imgIdx,
                        (int)TileState::prop,
                        Color(0.5f, 0.5f, 0.5f),
                        tileRoomIdx);
                }
            }
        }
    }

    void ProcedureMapGeneration::Spawner()
    {
        SetGateSpawner();
        SetTreasureSpawner();
        SetEnemySpawner();
    }

    void ProcedureMapGeneration::PropWall()
    {
        int roomIdx = 0;
        for (auto& elem : selectedRooms)
        {
            WrappingFuncHistogram(elem);

            if (xMax > 1 && yMax > 1)
            {
                SetTileRange(TileState::wall, propWallFloorImgMin, propWallFloorImgMax, propSour, propDest, roomIdx);
                SetTileAllDir(TileState::wall, propWallImgDir, propSour, propDest, roomIdx);
            }
            roomIdx++;
        }
        MAPINFO.tilemap->CreateTileCost();
    }

    void ProcedureMapGeneration::PropPit()
    {
        int roomIdx = 0;
        for (auto& elem : selectedRooms)
        {
            WrappingFuncHistogram(elem);

            if (xMax > 2 && yMax > 2)
            {
                SetTileRange(TileState::pit, propPitFloorImgMin, propPitFloorImgMax, propSour, propDest, roomIdx);
                SetTileAllDir(TileState::pit, propPitImgDir, propSour, propDest, roomIdx);
            }
            roomIdx++;
        }

        MAPINFO.tilemap->CreateTileCost();
    }

    void ProcedureMapGeneration::Finish()
    {
        //SaveLoad
        if (GUI.FileImGui("Save Room", "Save Map",
            ".txt", "../Contents/Room"))
        {
            string path = ImGuiFileDialog::Instance()->GetCurrentFileName();
            file = path;
            Save();
        }
        ImGui::SameLine();
        if (GUI.FileImGui("Load Room", "Load Map",
            ".txt", "../Contents/Room"))
        {
            string path = ImGuiFileDialog::Instance()->GetCurrentFileName();
            file = path;
            Load();
        }
    }

    void ProcedureMapGeneration::SetTileRange(const TileState tileState, const Vec2i imgMin, const Vec2i imgMax,
        const Vec2i sour, const Vec2i dest, const int roomIdx)
    {
        for (int y = sour.y + 1; y <= dest.y - 1; y++)
        {
            for (int x = sour.x + 1; x <= dest.x - 1; x++)
            {
                MAPINFO.tilemap->SetTile(Vec2i(x, y),
                    Vec2i(RANDOM.Int(imgMin.x, imgMax.x),
                        RANDOM.Int(imgMin.y, imgMax.y)),
                    MAPINFO.imgIdx,
                    (int)tileState,
                    Color(0.5f, 0.5f, 0.5f, 1.0f),
                    roomIdx);
            }
        }
    };

    void ProcedureMapGeneration::SetTileSpecificDir(const TileState tileState, const Vec2i imgDir[], 
        const Vec2i on, const DirState dir, const int roomIdx)
    {
        MAPINFO.tilemap->SetTile(on,
            imgDir[(int)dir],
            MAPINFO.imgIdx,
            (int)tileState,
            Color(0.5f, 0.5f, 0.5f),
            roomIdx,
            dir);
    }

    void ProcedureMapGeneration::SetTileAllDir(const TileState tileState, const Vec2i imgDir[], 
        const Vec2i sour, const Vec2i dest, const int roomIdx)
    {
        // wall - top, bottom, left, right, edge
        for (int x = sour.x + 1; x <= dest.x; x++)
        {
            SetTileSpecificDir(tileState, imgDir, Vec2i(x, dest.y), DirState::dirT, roomIdx);
            SetTileSpecificDir(tileState, imgDir, Vec2i(x, sour.y), DirState::dirB, roomIdx);
        }
        for (int y = sour.y + 1; y <= dest.y; y++)
        {
            SetTileSpecificDir(tileState, imgDir, Vec2i(sour.x, y), DirState::dirL, roomIdx);
            SetTileSpecificDir(tileState, imgDir, Vec2i(dest.x, y), DirState::dirR, roomIdx);
        }
        SetTileSpecificDir(tileState, imgDir, Vec2i(sour.x, dest.y), DirState::dirLT, roomIdx);
        SetTileSpecificDir(tileState, imgDir, Vec2i(dest.x, dest.y), DirState::dirRT, roomIdx);
        SetTileSpecificDir(tileState, imgDir, Vec2i(sour.x, sour.y), DirState::dirLB, roomIdx);
        SetTileSpecificDir(tileState, imgDir, Vec2i(dest.x, sour.y), DirState::dirRB, roomIdx);
    }

    void ProcedureMapGeneration::VisualizeSpawner()
    {
        // spawner 시각화
        int roomIdx = 0;
        for (auto& elem : selectedRooms)
        {
            if (elem->roomType == RoomType::enemy)
                for (auto& spawnerElem : elem->enemySpawner)
                    spawnerElem->isVisible = true;

            if (elem->roomType == RoomType::treasure)
                elem->treasureSpawner->isVisible = true;

            for (auto& spawnerElem : elem->gateSpawner)
                spawnerElem->isVisible = true;

            roomIdx++;
        }
    }

    void ProcedureMapGeneration::SetGateSpawner()
    {
        int idx = 0;
        for (auto& elem : selectedRooms)
        {
            int tileRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(elem->On());
            float xHalf = MAPINFO.tilemap->scale.x / 2.0f;
            float yHalf = MAPINFO.tilemap->scale.y / 2.0f;

            Vec2i sour = Vec2i(elem->On().x - 1, elem->On().y - 1);
            Vec2i dest = Vec2i(elem->On().x + 1, elem->On().y + 1);
            int curGateIdx = 0;

            for (int y = sour.y; y <= dest.y; y++)
            {
                for (int x = sour.x; x <= dest.x; x++)
                {
                    MAPINFO.tilemap->SetTile(Vec2i(x, y),
                        spawnerImg,
                        MAPINFO.imgIdx,
                        (int)TileState::spawner,
                        Color(0.5f, 0.5f, 0.5f),
                        tileRoomIdx);

                    Vector2 wpos = MAPINFO.tilemap->TileIdxToWorldPos(Vec2i(x, y));
                    elem->gateSpawner[curGateIdx]->SetWorldPos(Vector2(wpos.x + xHalf, wpos.y + yHalf));
                    elem->gateSpawner[curGateIdx]->isVisible = true;
                    curGateIdx++;
                }
            }

            idx++;
        }
    }

    void ProcedureMapGeneration::SetEnemySpawner()
    {
        int idx = 0;
        for (auto& elem : selectedRooms)
        {
            int tileRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(elem->On());
            float xHalf = MAPINFO.tilemap->scale.x / 2.0f;
            float yHalf = MAPINFO.tilemap->scale.y / 2.0f;

            if (idx != (int)RoomType::start && idx != (int)RoomType::treasure)
            {
                for (auto& spawnPosElem : elem->enemySpawner)
                {
                    Vec2i on;
                    Vector2 spawnerPos = spawnPosElem->GetWorldPos();

                    if (spawnerPos.x == 0.0f &&
                        spawnerPos.y == 0.0f)
                    {
                        while (MAPINFO.tilemap->GetTileState(on) != TileState::floor)
                        {
                            int rx = RANDOM.Int(elem->TileLB().x + 1, elem->TileRB().x - 1);
                            int ry = RANDOM.Int(elem->TileLB().y + 1, elem->TileLT().y - 1);
                            on = Vec2i(rx, ry);
                        }
                    }
                    else
                        MAPINFO.tilemap->WorldPosToTileIdx(spawnerPos, on);

                    MAPINFO.tilemap->SetTile(on,
                        spawnerImg,
                        MAPINFO.imgIdx,
                        (int)TileState::spawner,
                        Color(0.5f, 0.5f, 0.5f),
                        tileRoomIdx);

                    Vector2 wpos = MAPINFO.tilemap->TileIdxToWorldPos(on);
                    spawnPosElem->SetWorldPos(Vector2(wpos.x + xHalf, wpos.y + yHalf));
                    spawnPosElem->isVisible = true;
                }
            }

            idx++;
        }
    }

    void ProcedureMapGeneration::SetTreasureSpawner()
    {
        shared_ptr<Room> treasure = selectedRooms[(int)RoomType::treasure];
        treasure->roomType = RoomType::treasure;
        treasure->cleared = true;

        Vec2i on;
        MAPINFO.tilemap->WorldPosToTileIdx(treasure->Pos(), on);
        Vector2 wpos = MAPINFO.tilemap->TileIdxToWorldPos(Vec2i(on.x, on.y));
        float tileXHalf = MAPINFO.tilemap->scale.x / 2.0f;
        float tileYHalf = MAPINFO.tilemap->scale.y / 2.0f;
        treasure->treasureSpawner->SetWorldPos(Vector2(wpos.x + tileXHalf, wpos.y + tileYHalf));
        treasure->treasureSpawner->isVisible = true;
        treasure->treasureSpawner->Update();

        MAPINFO.tilemap->WorldPosToTileIdx(wpos, on);
        MAPINFO.tilemap->SetTile(on,
            spawnerImg,
            MAPINFO.imgIdx,
            (int)TileState::spawner,
            Color(0.5f, 0.5f, 0.5f),
            (int)RoomType::treasure);
    }

    /*
    void ProcedureMapGeneration::MaximalSquare(const Room* elem)
    {
        const int size = RANDOM.Int(2, 4);
        int roomIdx = MAPINFO.tilemap->GetTileRoomIndex(elem->On());

        int xStart = elem->TileLB().x + 2;
        int xEnd = elem->TileRB().x - 2;
        int yStart = elem->TileLB().y + 2;
        int yEnd = elem->TileLT().y - 2;
        int w = elem->TileWidth() - 4;
        int h = elem->TileHeight() - 4;

        // Maximal Square Algorithm
        vector<int> dp(w + 2);
        int prev = 0, length = 0;

        bool flagLoopBreak = false;

        for (int y = 1; y <= h && false == flagLoopBreak; y++)
        {
            for (int x = 1; x <= w && false == flagLoopBreak; x++)
            {
                int temp = dp[x];
                Vec2i on = Vec2i(xStart + x - 1, yStart + y - 1);
                if (MAPINFO.tilemap->GetTileState(on) == TileState::floor)
                {
                    dp[x] = min(min(dp[x - 1], prev), dp[x]) + 1;
                    length = max(length, dp[x]);
                }
                else
                    dp[x] = 0;
                prev = temp;

                if (length >= size)
                {
                    Vec2i sour, dest;
                    sour = Vec2i(xStart + x - size + 1, yStart + y);
                    dest = Vec2i(xStart + x, yStart + y + size - 1);

                    SetTileAllDir(TileState::wall, wallImgDir, sour, dest, roomIdx);

                    flagLoopBreak = true;

                    length = 0;
                }
            }
        }

        cout << length << '\n';
    }
    */

    void ProcedureMapGeneration::WrappingFuncHistogram(const shared_ptr<Room> elem)
    {
        int xStart = elem->TileLB().x + 2;
        int xEnd = elem->TileRB().x - 1;
        int yStart = elem->TileLB().y + 2;
        int yEnd = elem->TileLT().y - 1;
        int xSize = elem->TileWidth() - 3;
        int ySize = elem->TileHeight() - 3;

        propSour = Vec2i(0, 0);
        propDest = Vec2i(0, 0);
        xMax = 0;
        yMax = 0;

        HISTOGRAM.FindMaximalRectangleForStack(xStart, xEnd, yStart, yEnd, xSize, ySize,
            xMax, yMax, propSour, propDest);
    }


    void ProcedureMapGeneration::Save()
    {
        ofstream fout;
        string path = "../Contents/Room/" + file;

        fout.open(path.c_str(), ios::out);

        if (fout.is_open())
        {
            int roomIdx = 0;
            for (auto& roomElem : selectedRooms)
            {
                fout << "roomIdx " << roomIdx << '\n';

                fout << "colPos " << roomElem->Pos().x << " " << roomElem->Pos().y << '\n';
                fout << "colScale " << roomElem->col->scale.x << " " << roomElem->col->scale.y << '\n';

                for (auto& spawnerElem : roomElem->enemySpawner)
                {
                    fout << "enemySpawner " << spawnerElem->GetWorldPos().x << " " << 
                        spawnerElem->GetWorldPos().y << '\n';
                }

                fout << "treasureSpawner " << roomElem->treasureSpawner->GetWorldPos().x << " " <<
                    roomElem->treasureSpawner->GetWorldPos().y << '\n';

                for (auto& spawnerElem : roomElem->gateSpawner)
                {
                    fout << "gateSpawner " << spawnerElem->GetWorldPos().x << " " <<
                        spawnerElem->GetWorldPos().y << '\n';
                }

                fout << "cleared " << (int)roomElem->cleared << '\n';
                fout << "roomType " << (int)roomElem->roomType << '\n';

                fout << "doorTileNum " << roomElem->doorTileIdxs.size() << '\n';

                for (auto& doorTileIdx : roomElem->doorTileIdxs)
                    fout << "doorTileIdx " << doorTileIdx.x << " " << doorTileIdx.y << '\n';

                fout << '\n';
                roomIdx++;
            }
        }

        fout.close();
    }

    void ProcedureMapGeneration::Load()
    {
        for (auto& elem : selectedRooms)
            elem.reset();

        selectedRooms.clear();


        ifstream fin;
        string path = "../Contents/Room/" + file;

        fin.open(path.c_str(), ios::in);

        string temp;

        if (fin.is_open())
        {
            int roomIdx = 0;
            while (1)
            {
                Room* room = new Room;

                // index
                fin >> temp >> temp;
                if (fin.eof()) break;

                // colPos
                float x, y;
                fin >> temp >> x >> y;
                room->SetPos(Vector2(x, y));

                // colScale
                fin >> temp >> room->col->scale.x >> room->col->scale.y;

                // enemySpawner
                if (roomIdx == (int)RoomType::start || roomIdx == (int)RoomType::treasure)
                {
                    for (auto& spawnerElem : room->enemySpawner)
                        fin >> temp >> temp >> temp;
                }
                else
                {
                    int spawnerIdx = 0;
                    for (auto& spawnerElem : room->enemySpawner)
                    {
                        float x, y;
                        fin >> temp >> x >> y;
                        room->enemySpawner[spawnerIdx]->SetWorldPos(Vector2(x, y));
                        spawnerIdx++;
                    }
                }

                // treasureSpawner
                if (roomIdx == (int)RoomType::treasure)
                {
                    float x, y;
                    fin >> temp >> x >> y;
                    room->treasureSpawner->SetWorldPos(Vector2(x, y));
                }
                else
                    fin >> temp >> temp >> temp;

                // gateSpawner
                int spawnerIdx = 0;
                for (auto& spawnerElem : room->gateSpawner)
                {
                    float x, y;
                    fin >> temp >> x >> y;
                    room->gateSpawner[spawnerIdx]->SetWorldPos(Vector2(x, y));
                    spawnerIdx++;
                }

                // cleared
                int cleared;
                fin >> temp >> cleared;
                room->cleared = cleared;
                
                // roomType
                int type;
                fin >> temp >> type;
                room->roomType = (RoomType)type;

                // doorTileNum
                int n;
                fin >> temp >> n;

                // doorTileIdx
                for (int i = 0; i < n; i++)
                {
                    float x, y;
                    fin >> temp >> x >> y;
                    room->doorTileIdxs.emplace_back(Vec2i(static_cast<int>(x), static_cast<int>(y)));
                }

                selectedRooms.emplace_back(room);
                roomIdx++;
            }
        }

        for (auto& elem : selectedRooms)
        {
            elem->col->isVisible = false;
            elem->Update();
        }

        SetGateSpawner();
        SetTreasureSpawner();
        SetEnemySpawner();

        fin.close();
    }
}