#include "framework.h"

bool AStar::FindPath(std::shared_ptr<ObTileMap> tilemap, Vec2i sour, Vec2i dest, OUT vector<Tile>& way, bool checkDiagnoal)
{
    //���߿� �ϳ��� ���̸� �� �� �ִ±��� ����.  -> �� �� �ְ� ����
    //if (Tiles[dest.x][dest.y].state == TileState::wall)
    //{
    //    return false;
    //}

    //������ �ִ� ���� ���� ����.
    way.clear();

    //����� ������ ������
    if (sour == dest)
        return false;

    Vec2i tileSize = tilemap->GetTileSize();

    //Ÿ�� ��� �ʱ�ȭ
    for (int i = 0; i < tileSize.x; i++)
        for (int j = 0; j < tileSize.y; j++)
            tilemap->Tiles[i][j].ClearCost();

    //�켱���� ť
    //            ����Ÿ��, �����̳�Ÿ��, ���Լ�
    priority_queue<PTile, vector<PTile>, ComparePTile> List;

    //����Ʈ�� ������� �߰�
    Tile& tileRef = tilemap->Tiles[sour.x][sour.y];
    tileRef.G = 0;          //����� �������� 0
    tileRef.ClacH(dest);    //���������� ������ �����
    tileRef.ClacF();        //�ѿ����� �����

    //          �ּ�,  ������
    // List.push({ pTemp, pTemp->F });
    List.push({ tileRef, tileRef.F });

    //���� Ÿ�� ���˻�
    vector<Vec2i> LoopIdx;

    while (1)
    {
        //���� F���� ���ٴ� 
        if (List.empty())
            return false;

        //Ž����� �޾ƿ���
        PTile node = List.top();

        //Ž����Ͽ��� ����
        node.first.isFind = true;

        //������ ��������
        List.pop();

        //�������� �������̸� ����
        if (node.first.idx == dest)
            break;

        //L Ÿ���� ������ ��
        if (node.first.idx.x > 0)
        {
            Vec2i on = Vec2i(node.first.idx.x - 1,
                node.first.idx.y);
            LoopIdx.push_back(on);
        }

        //T Ÿ���� ������ ��
        if (node.first.idx.y > 0)
        {
            Vec2i on = Vec2i(node.first.idx.x,
                node.first.idx.y - 1);
            LoopIdx.push_back(on);
        }

        //R Ÿ���� ������ ��
        if (node.first.idx.x < tileSize.x - 1)
        {
            Vec2i on = Vec2i(node.first.idx.x + 1,
                node.first.idx.y);
            LoopIdx.push_back(on);
        }

        //D Ÿ���� ������ ��
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

                // ���� ���� ���� �� �밢������ ���� �ʱ�
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

        //�����¿�(�밢��)Ÿ�� ���˻�
        for (int i = 0; i < LoopIdx.size(); i++)
        {
            Tile& loopTile = tilemap->Tiles[LoopIdx[i].x][LoopIdx[i].y];

            //���� �ƴҶ�
            switch (loopTile.state)
            {
            case TileState::wall:
            case TileState::door:
            case TileState::pit:
                break;

            default:
                //������ �����
                loopTile.ClacH(dest);

                //���� �������ִ� ����� Ŭ��
                if (loopTile.G > node.first.G + 10)
                {
                    //��밻��
                    loopTile.G = node.first.G + 10;
                    loopTile.ClacF();

                    //�����κ��� �������� ǥ��
                    loopTile.P = make_shared<Tile>(node.first);

                    //�� Ÿ���� ã���� ���� Ÿ���̸� ����Ʈ�� �߰�
                    if (!loopTile.isFind)
                        List.push({ loopTile, loopTile.F });
                }
                break;
            }
        }
        LoopIdx.clear();
    }

    //�������� ������ �Ǿ�����
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