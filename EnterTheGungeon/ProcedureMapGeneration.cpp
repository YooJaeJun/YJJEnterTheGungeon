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
                autoProcess = true;
        }

        switch (state)
        {
        case MapGenState::spray:
            Spray();
            break;

        case MapGenState::spread:
            Spread();
            break;

        case MapGenState::select:
            Select();
            break;

        case MapGenState::triangulate:
            Triangulate();
            break;

        case MapGenState::span:
            Spanning();
            break;

        case MapGenState::loop:
            Loop();
            break;

        case MapGenState::clean:
            Clean();
            break;

        case MapGenState::roomTile:
            RoomTile();
            break;

        case MapGenState::passageTile:
            PassageTile();
            break;

        case MapGenState::passagePitTile:
            PassagePitTile();
            break;

        case MapGenState::prop:
            Prop();
            break;

        case MapGenState::spawner:
            Spawner();
            break;

        case MapGenState::propPit:
            PropPit();
            break;

        case MapGenState::propWall:
            PropWall();
            MAPINFO.useGui = true;
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

            map<Int2, int> dic;
            int xRand = 0, yRand = 0;
            do {
                xRand = RANDOM.Int(-10, 10);
                yRand = RANDOM.Int(-10, 10);
            } while (dic[Int2(xRand, yRand)]);

            dic[Int2(xRand, yRand)]++;

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
                Int2 on;
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
            roomNode.emplace_back(ObNode(idx, elem->Pos()));
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
        Int2 sour, dest;
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
                if (AlmostEqualVector2(v, selectedRooms[roomIdx]->Pos()))
                    nodesForRoomIndex[v] = roomIdx;

                Vector2 w = Vector2(elem.W().x, elem.W().y);
                if (AlmostEqualVector2(w, selectedRooms[roomIdx]->Pos()))
                    nodesForRoomIndex[w] = roomIdx;
            }
        }

        // 1. 8방향 체크해 문의 방향을 정하고, 2. 방향에 따라 다른 이미지, 3. 모서리에 위치할 시 예외처리
        auto SetDoor = [&](Int2 on, const int roomIdx, const DirState dir)
        {
            deque<bool> dirWall(8);

            for (int i = 0; i < 8; i++)
            {
                int nx = on.x + dx[i];
                int ny = on.y + dy[i];

                if (MAPINFO.tilemap->GetTileState(Int2(nx, ny)) == TileState::wall ||
                    MAPINFO.tilemap->GetTileState(Int2(nx, ny)) == TileState::door)
                    dirWall[i] = true;
            }

            Int2 doorTileIdx = on;
            Int2 floorTileIdx;

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
                    Int2(RANDOM.Int(floorImgMin.x, floorImgMax.x),
                        RANDOM.Int(floorImgMin.y, floorImgMax.y)),
                    MAPINFO.imgIdx,
                    (int)TileState::floor, 
                    Color(0.5f, 0.5f, 0.5f),
                    -1, 
                    dir);
            }

            MAPINFO.tilemap->SetTile(doorTileIdx,
                Int2(RANDOM.Int(floorImgMin.x, floorImgMax.x),
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

        auto FindValidLoad = [&](Int2 sour, Int2 dest, bool first, const int passageIdx) ->Int2
        {
            if (ASTAR.FindPath(MAPINFO.tilemap, sour, dest, way, false))
            {
                int sourRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(sour);
                int destRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(dest);

                Int2 beforeOn = way.back().idx;
                int beforeRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(beforeOn);
                DirState beforeDir = MAPINFO.tilemap->GetTileDir(beforeOn);

                for (int cur = way.size() - 2; cur >= 0; cur--)
                {
                    Int2 curOn = way[cur].idx;
                    TileState curTileState = MAPINFO.tilemap->GetTileState(curOn);
                    int curRoomIdx = MAPINFO.tilemap->GetTileRoomIndex(curOn);
                    DirState curDir = MAPINFO.tilemap->GetTileDir(curOn);

                    if (curTileState == TileState::none)
                    {
                        MAPINFO.tilemap->SetTile(curOn,
                            Int2(RANDOM.Int(floorImgMin.x, floorImgMax.x),
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
            const ObNode& v = elem.V();
            const ObNode& w = elem.W();

            ObNode mid = ObNode((w.x + v.x) / 2.0f, (w.y + v.y) / 2.0f);

            // Vector2 vScaleHalf = selectedRooms[nodesForRoomIndex[v]]->col->scale / 2.0f;
            // Vector2 wScaleHalf = selectedRooms[nodesForRoomIndex[w]]->col->scale / 2.0f;

            Vector2	start = Vector2(v.x, v.y);
            Vector2 end = Vector2(w.x, w.y);

            Int2 sour, dest;
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

                    if (MAPINFO.tilemap->GetTileState(Int2(nx, ny)) == TileState::none)
                    {
                        MAPINFO.tilemap->SetTile(Int2(nx, ny),
                            Int2(RANDOM.Int(passagePitImgMin.x, passagePitImgMax.x),
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
            Int2 on;
            MAPINFO.tilemap->WorldPosToTileIdx(elem->Pos(), on);
            Int2 scaleOn = Int2(static_cast<int>(elem->col->scale.x / 100), static_cast<int>(elem->col->scale.y / 100));
            Int2 halfScaleOn = scaleOn / 2;

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

                if (MAPINFO.tilemap->GetTileState(Int2(rx, ry)) == TileState::floor)
                {
                    MAPINFO.tilemap->SetTile(Int2(rx, ry),
                        Int2(RANDOM.Int(propImgMin.x, propImgMax.x),
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
            propSour = Int2(0, 0);
            propDest = Int2(0, 0);
            areaMax = 0;
            xMax = 0;
            yMax = 0;

            Histogram(elem);

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
            propSour = Int2(0, 0);
            propDest = Int2(0, 0);
            areaMax = 0;
            xMax = 0;
            yMax = 0;

            Histogram(elem);

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

    void ProcedureMapGeneration::SetTileRange(const TileState tileState, const Int2 imgMin, const Int2 imgMax, 
        const Int2 sour, const Int2 dest, const int roomIdx)
    {
        for (int y = sour.y + 1; y <= dest.y - 1; y++)
        {
            for (int x = sour.x + 1; x <= dest.x - 1; x++)
            {
                MAPINFO.tilemap->SetTile(Int2(x, y),
                    Int2(RANDOM.Int(imgMin.x, imgMax.x),
                        RANDOM.Int(imgMin.y, imgMax.y)),
                    MAPINFO.imgIdx,
                    (int)tileState,
                    Color(0.5f, 0.5f, 0.5f, 1.0f),
                    roomIdx);
            }
        }
    };

    void ProcedureMapGeneration::SetTileSpecificDir(const TileState tileState, const Int2 imgDir[], 
        const Int2 on, const DirState dir, const int roomIdx)
    {
        MAPINFO.tilemap->SetTile(on,
            imgDir[(int)dir],
            MAPINFO.imgIdx,
            (int)tileState,
            Color(0.5f, 0.5f, 0.5f),
            roomIdx,
            dir);
    }

    void ProcedureMapGeneration::SetTileAllDir(const TileState tileState, const Int2 imgDir[], 
        const Int2 sour, const Int2 dest, const int roomIdx)
    {
        // wall - top, bottom, left, right, edge
        for (int x = sour.x + 1; x <= dest.x; x++)
        {
            SetTileSpecificDir(tileState, imgDir, Int2(x, dest.y), DirState::dirT, roomIdx);
            SetTileSpecificDir(tileState, imgDir, Int2(x, sour.y), DirState::dirB, roomIdx);
        }
        for (int y = sour.y + 1; y <= dest.y; y++)
        {
            SetTileSpecificDir(tileState, imgDir, Int2(sour.x, y), DirState::dirL, roomIdx);
            SetTileSpecificDir(tileState, imgDir, Int2(dest.x, y), DirState::dirR, roomIdx);
        }
        SetTileSpecificDir(tileState, imgDir, Int2(sour.x, dest.y), DirState::dirLT, roomIdx);
        SetTileSpecificDir(tileState, imgDir, Int2(dest.x, dest.y), DirState::dirRT, roomIdx);
        SetTileSpecificDir(tileState, imgDir, Int2(sour.x, sour.y), DirState::dirLB, roomIdx);
        SetTileSpecificDir(tileState, imgDir, Int2(dest.x, sour.y), DirState::dirRB, roomIdx);
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

            Int2 sour = Int2(elem->On().x - 1, elem->On().y - 1);
            Int2 dest = Int2(elem->On().x + 1, elem->On().y + 1);
            int curGateIdx = 0;

            for (int y = sour.y; y <= dest.y; y++)
            {
                for (int x = sour.x; x <= dest.x; x++)
                {
                    MAPINFO.tilemap->SetTile(Int2(x, y),
                        spawnerImg,
                        MAPINFO.imgIdx,
                        (int)TileState::spawner,
                        Color(0.5f, 0.5f, 0.5f),
                        tileRoomIdx);

                    Vector2 wpos = MAPINFO.tilemap->TileIdxToWorldPos(Int2(x, y));
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
                    Int2 on;
                    Vector2 spawnerPos = spawnPosElem->GetWorldPos();

                    if (spawnerPos.x == 0.0f &&
                        spawnerPos.y == 0.0f)
                    {
                        while (MAPINFO.tilemap->GetTileState(on) != TileState::floor)
                        {
                            int rx = RANDOM.Int(elem->TileLB().x + 1, elem->TileRB().x - 1);
                            int ry = RANDOM.Int(elem->TileLB().y + 1, elem->TileLT().y - 1);
                            on = Int2(rx, ry);
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

        Int2 on;
        MAPINFO.tilemap->WorldPosToTileIdx(treasure->Pos(), on);
        Vector2 wpos = MAPINFO.tilemap->TileIdxToWorldPos(Int2(on.x, on.y));
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
                Int2 on = Int2(xStart + x - 1, yStart + y - 1);
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
                    Int2 sour, dest;
                    sour = Int2(xStart + x - size + 1, yStart + y);
                    dest = Int2(xStart + x, yStart + y + size - 1);

                    SetTileAllDir(TileState::wall, wallImgDir, sour, dest, roomIdx);

                    flagLoopBreak = true;

                    length = 0;
                }
            }
        }

        cout << length << '\n';
    }
    */

    void ProcedureMapGeneration::Histogram(const shared_ptr<Room> elem)
    {
        int xStart = elem->TileLB().x + 2;
        int xEnd = elem->TileRB().x - 1;
        int yStart = elem->TileLB().y + 2;
        int yEnd = elem->TileLT().y - 1;
        int xSize = elem->TileWidth() - 3;
        int ySize = elem->TileHeight() - 3;
        vector<int> height(xSize + 1);
        bool flagLoopBreak = false;

        for (int y = 0; y < ySize && false == flagLoopBreak; y++)
        {
            stack<int> st;
            for (int x = 0; x <= xSize; x++)
            {
                if (x < xSize)
                {
                    Int2 on = Int2(xStart + x, yStart + y);
                    if (MAPINFO.tilemap->GetTileState(on) == TileState::floor)
                        height[x]++;
                    else 
                        height[x] = 0;
                }

                while (!st.empty() && height[x] < height[st.top()])
                {
                    int h = height[st.top()];
                    st.pop();
                    int w = (st.empty()) ? x : x - 1 - st.top();

                    if (areaMax <= w * h)
                    {
                        areaMax = w * h;
                        xMax = w;
                        yMax = h;
                        propSour = Int2(xStart + x - w, yStart + y - h + 1);
                        propDest = Int2(xStart + x - 1, yStart + y);
                    }
                }
                st.push(x);
            }
        }
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
                    room->doorTileIdxs.emplace_back(Int2(static_cast<int>(x), static_cast<int>(y)));
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