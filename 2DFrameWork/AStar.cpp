#include "framework.h"

bool AStar::FindPath(std::shared_ptr<ObTileMap> tilemap, Vec2i sour, Vec2i dest, OUT vector<Tile>& way, bool checkDiagnoal)
{
    //둘중에 하나가 벽이면 갈 수 있는길이 없다.  -> 갈 수 있게 수정
    //if (Tiles[dest.x][dest.y].state == TileState::wall)
    //{
    //    return false;
    //}

    //기존에 있던 길은 전부 비운다.
    way.clear();

    //출발지 목적지 같으면
    if (sour == dest)
        return false;

    Vec2i tileSize = tilemap->GetTileSize();

    //타일 비용 초기화
    for (int i = 0; i < tileSize.x; i++)
        for (int j = 0; j < tileSize.y; j++)
            tilemap->Tiles[i][j].ClearCost();

    //우선순위 큐
    //            원소타입, 컨테이너타입, 비교함수
    priority_queue<PTile, vector<PTile>, ComparePTile> List;

    //리스트에 출발지를 추가
    Tile& tileRef = tilemap->Tiles[sour.x][sour.y];
    tileRef.G = 0;          //출발지 현재비용은 0
    tileRef.ClacH(dest);    //목적지까지 예상비용 만들기
    tileRef.ClacF();        //총예상비용 만들기

    //          주소,  예상비용
    // List.push({ pTemp, pTemp->F });
    List.push({ tileRef, tileRef.F });

    //인접 타일 비용검사
    vector<Vec2i> LoopIdx;

    while (1)
    {
        //꺼낼 F값이 없다는 
        if (List.empty())
            return false;

        //탐색노드 받아오기
        PTile node = List.top();

        //탐색목록에서 제외
        node.first.isFind = true;

        //맨윗값 빼버리기
        List.pop();

        //맨윗값이 도착점이면 종료
        if (node.first.idx == dest)
            break;

        //L 타일이 존재할 때
        if (node.first.idx.x > 0)
        {
            Vec2i on = Vec2i(node.first.idx.x - 1,
                node.first.idx.y);
            LoopIdx.push_back(on);
        }

        //T 타일이 존재할 때
        if (node.first.idx.y > 0)
        {
            Vec2i on = Vec2i(node.first.idx.x,
                node.first.idx.y - 1);
            LoopIdx.push_back(on);
        }

        //R 타일이 존재할 때
        if (node.first.idx.x < tileSize.x - 1)
        {
            Vec2i on = Vec2i(node.first.idx.x + 1,
                node.first.idx.y);
            LoopIdx.push_back(on);
        }

        //D 타일이 존재할 때
        if (node.first.idx.y < tileSize.y - 1)
        {
            Vec2i on = Vec2i(node.first.idx.x,
                node.first.idx.y + 1);
            LoopIdx.push_back(on);
        }

        if (checkDiagnoal)
        {
            //LB
            if (node.first.idx.x > 0 &&
                node.first.idx.y < tileSize.y - 1)
            {
                Vec2i on = Vec2i(node.first.idx.x - 1,
                    node.first.idx.y + 1);

                // 주위 벽이 있을 땐 대각선으로 가지 않기
                TileState state1 = tilemap->GetTileState(Vec2i(on.x - 1, on.y));
                TileState state2 = tilemap->GetTileState(Vec2i(on.x, on.y - 1));
                if (state1 != TileState::wall &&
                    state2 != TileState::wall &&
                    state1 != TileState::pit &&
                    state2 != TileState::pit)
                    LoopIdx.push_back(on);
            }

            //LT
            if (node.first.idx.x > 0 &&
                node.first.idx.y > 0)
            {
                Vec2i on = Vec2i(node.first.idx.x - 1,
                    node.first.idx.y - 1);

                TileState state1 = tilemap->GetTileState(Vec2i(on.x - 1, on.y));
                TileState state2 = tilemap->GetTileState(Vec2i(on.x, on.y + 1));
                if (state1 != TileState::wall &&
                    state2 != TileState::wall &&
                    state1 != TileState::pit &&
                    state2 != TileState::pit)
                    LoopIdx.push_back(on);
            }

            //RB
            if (node.first.idx.x < tileSize.x - 1 &&
                node.first.idx.y < tileSize.y - 1)
            {
                Vec2i on = Vec2i(node.first.idx.x + 1,
                    node.first.idx.y + 1);

                TileState state1 = tilemap->GetTileState(Vec2i(on.x + 1, on.y));
                TileState state2 = tilemap->GetTileState(Vec2i(on.x, on.y - 1));
                if (state1 != TileState::wall &&
                    state2 != TileState::wall &&
                    state1 != TileState::pit &&
                    state2 != TileState::pit)
                    LoopIdx.push_back(on);
            }

            //RT
            if (node.first.idx.x < tileSize.x - 1 &&
                node.first.idx.y > 0)
            {
                Vec2i on = Vec2i(node.first.idx.x + 1,
                    node.first.idx.y - 1);

                TileState state1 = tilemap->GetTileState(Vec2i(on.x + 1, on.y));
                TileState state2 = tilemap->GetTileState(Vec2i(on.x, on.y + 1));
                if (state1 != TileState::wall &&
                    state2 != TileState::wall &&
                    state1 != TileState::pit &&
                    state2 != TileState::pit)
                    LoopIdx.push_back(on);
            }
        }

        //상하좌우(대각선)타일 비용검사
        for (int i = 0; i < LoopIdx.size(); i++)
        {
            Tile& loopTile = tilemap->Tiles[LoopIdx[i].x][LoopIdx[i].y];

            //벽이 아닐때
            switch (loopTile.state)
            {
            case TileState::wall:
            case TileState::door:
            case TileState::pit:
                break;

            default:
                //예상비용 만들기
                loopTile.ClacH(dest);

                //현재 가지고있는 비용이 클때
                if (loopTile.G > node.first.G + 10)
                {
                    //비용갱신
                    loopTile.G = node.first.G + 10;
                    loopTile.ClacF();

                    //누구로부터 갱신인지 표시
                    loopTile.P = make_shared<Tile>(node.first);

                    //이 타일이 찾은적 없던 타일이면 리스트에 추가
                    if (!loopTile.isFind)
                        List.push({ loopTile, loopTile.F });
                }
                break;
            }
        }
        LoopIdx.clear();
    }

    //도착지가 설정이 되었을때
    auto pTile = make_shared<Tile>(tilemap->Tiles[dest.x][dest.y]);
    while (1)
    {
        way.emplace_back(*pTile);

        pTile = pTile->P;

        if (pTile == nullptr)
            break;
    }
    return true;
}