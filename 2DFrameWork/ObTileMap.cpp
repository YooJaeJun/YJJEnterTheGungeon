#include "framework.h"

ObTileMap::ObTileMap()
{
    SafeRelease(vertexBuffer);
    SafeDeleteArray(vertices);

    //타일 가로 세로 개수
    tileSize.x = 20;
    tileSize.y = 20;

    file = "EnterTheGungeon.txt";
    //하나의 타일 크기
    scale = Vector2(50.0f, 50.0f);

    tileImages[0] = make_shared<ObImage>(L"Empty.png");
    tileImages[0]->maxFrame = Int2(10, 10);

    tileImages[1] = make_shared<ObImage>(L"EnterTheGungeon/Level/Tileset.png");
    tileImages[1]->maxFrame = Int2(10, 10);

    ResizeTile(tileSize);
}

ObTileMap::~ObTileMap()
{
    SafeRelease(vertexBuffer);
    SafeDeleteArray(vertices);
}

void ObTileMap::CreateTileCost()
{
    for (int i = 0; i < Tiles.size(); i++)
    {
        Tiles[i].clear();
    }
    Tiles.clear();

    Tiles.resize(tileSize.x);
    for (int i = 0; i < Tiles.size(); i++)
    {
        Tiles[i].resize(tileSize.y);
    }

    Vector2 half = scale * 0.5f;
    Update();
    for (int i = 0; i < tileSize.x; i++)
    {
        for (int j = 0; j < tileSize.y; j++)
        {
            Tiles[i][j].idx = Int2(i, j);
            Tiles[i][j].state = GetTileState(Tiles[i][j].idx);
            Tiles[i][j].roomIdx = GetTileRoomIndex(Tiles[i][j].idx);
            Tiles[i][j].dir = GetTileDir(Tiles[i][j].idx);

            Tiles[i][j].Pos.x = i * scale.x + GetWorldPos().x + half.x;
            Tiles[i][j].Pos.y = j * scale.y + GetWorldPos().y + half.y;
        }
    }
}

//사이즈 재조정, 정정 재조정
void ObTileMap::ResizeTile(Int2 newTileSize)
{
    VertexTile* newVertices = new VertexTile[newTileSize.x * newTileSize.y * 6];

    //세로
    for (int i = 0; i < newTileSize.y; i++)
    {
        //가로
        for (int j = 0; j < newTileSize.x; j++)
        {
            int tileIdx = newTileSize.x * i + j;
            //0
            newVertices[tileIdx * 6].position.x = 0.0f + j;
            newVertices[tileIdx * 6].position.y = 0.0f + i;
            newVertices[tileIdx * 6].uv = Vector2(0.0f, 1.0f);
            //1                             
            newVertices[tileIdx * 6 + 1].position.x = 0.0f + j;
            newVertices[tileIdx * 6 + 1].position.y = 1.0f + i;
            newVertices[tileIdx * 6 + 1].uv = Vector2(0.0f, 0.0f);
            //2                             
            newVertices[tileIdx * 6 + 2].position.x = 1.0f + j;
            newVertices[tileIdx * 6 + 2].position.y = 0.0f + i;
            newVertices[tileIdx * 6 + 2].uv = Vector2(1.0f, 1.0f);
            //3
            newVertices[tileIdx * 6 + 3].position.x = 1.0f + j;
            newVertices[tileIdx * 6 + 3].position.y = 1.0f + i;
            newVertices[tileIdx * 6 + 3].uv = Vector2(1.0f, 0.0f);
            //4
            newVertices[tileIdx * 6 + 4].position.x = 1.0f + j;
            newVertices[tileIdx * 6 + 4].position.y = 0.0f + i;
            newVertices[tileIdx * 6 + 4].uv = Vector2(1.0f, 1.0f);
            //5
            newVertices[tileIdx * 6 + 5].position.x = 0.0f + j;
            newVertices[tileIdx * 6 + 5].position.y = 1.0f + i;
            newVertices[tileIdx * 6 + 5].uv = Vector2(0.0f, 0.0f);
        }
    }

    //Copy
    if (vertices)
    {
        Int2 Min = Int2(min(newTileSize.x, tileSize.x), min(newTileSize.y, tileSize.y));
        for (int i = 0; i < Min.y; i++)
        {
            for (int j = 0; j < Min.x; j++)
            {
                int SrcIdx = tileSize.x * i + j;
                int DestIdx = newTileSize.x * i + j;

                for (int k = 0; k < 6; k++)
                {
                    newVertices[DestIdx * 6 + k] = vertices[SrcIdx * 6 + k];
                }
            }
        }
    }

    SafeDeleteArray(vertices);
    vertices = newVertices;
    tileSize = newTileSize;

    SafeRelease(vertexBuffer);
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(VertexTile) * tileSize.x * tileSize.y * 6;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        Check(hr);
    }
}

void ObTileMap::RenderGui(Int2& guiPickingIdx, int& imgIdx)
{
    if (ImGui::InputInt("ImgIdx", &imgIdx))
    {
        if (imgIdx > imageCount - 1)
        {
            imgIdx = 0; //첫번째이미지로 돌리기
        }
        else if (imgIdx < 0)
        {
            //마지막 이미지로 돌리기
            imgIdx = imageCount - 1;
        }
    }

    Int2 MF = tileImages[imgIdx]->maxFrame;
    ImVec2 size;
    size.x = 200.0f / (float)MF.x;
    size.y = 200.0f / (float)MF.y;

    //텍스쳐 좌표
    ImVec2 LT, RB;
    int index = 0;

    for (UINT i = 0; i < MF.y; i++)
    {
        for (UINT j = 0; j < MF.x; j++)
        {
            if (j != 0)
            {
                //같은줄에 배치
                ImGui::SameLine();
            }

            //텍스쳐 좌표
            LT.x = 1.0f / MF.x * j;
            LT.y = 1.0f / MF.y * i;
            RB.x = 1.0f / MF.x * (j + 1);
            RB.y = 1.0f / MF.y * (i + 1);

            ImGui::PushID(index);
            if (ImGui::ImageButton((void*)tileImages[imgIdx]->SRV, size, LT, RB))
            {
                guiPickingIdx.x = j;
                guiPickingIdx.y = i;
            }
            index++;
            ImGui::PopID();
        }
    }
}

void ObTileMap::Render()
{
    if (!isVisible)return;
    GameObject::Render();

    for (int i = 0; i < imageCount; i++)
    {
        if (tileImages[i])
        {
            D3D->GetDC()->PSSetShaderResources(i, 1, &tileImages[i]->SRV);
            D3D->GetDC()->PSSetSamplers(i, 1, &tileImages[i]->sampler);
        }
    }

    tileMapShader->Set();

    UINT stride = sizeof(VertexTile);
    UINT offset = 0;

    //버텍스버퍼 바인딩
    D3D->GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    D3D->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    D3D->GetDC()->Draw(tileSize.x * tileSize.y * 6, 0);
}

void ObTileMap::SetTile(Int2 tileIdx, Int2 frameIdx, int imgIdx, int tileState, Color color, int roomIdx, DirState dir)
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;

    vertices[verticeIdx * 6 + 0].uv.x = frameIdx.x / (float)tileImages[imgIdx]->maxFrame.x;
    vertices[verticeIdx * 6 + 1].uv.x = frameIdx.x / (float)tileImages[imgIdx]->maxFrame.x;
    vertices[verticeIdx * 6 + 5].uv.x = frameIdx.x / (float)tileImages[imgIdx]->maxFrame.x;

    vertices[verticeIdx * 6 + 2].uv.x = (frameIdx.x + 1.0f) / (float)tileImages[imgIdx]->maxFrame.x;
    vertices[verticeIdx * 6 + 3].uv.x = (frameIdx.x + 1.0f) / (float)tileImages[imgIdx]->maxFrame.x;
    vertices[verticeIdx * 6 + 4].uv.x = (frameIdx.x + 1.0f) / (float)tileImages[imgIdx]->maxFrame.x;

    vertices[verticeIdx * 6 + 3].uv.y = frameIdx.y / (float)tileImages[imgIdx]->maxFrame.y;
    vertices[verticeIdx * 6 + 1].uv.y = frameIdx.y / (float)tileImages[imgIdx]->maxFrame.y;
    vertices[verticeIdx * 6 + 5].uv.y = frameIdx.y / (float)tileImages[imgIdx]->maxFrame.y;

    vertices[verticeIdx * 6 + 2].uv.y = (frameIdx.y + 1.0f) / (float)tileImages[imgIdx]->maxFrame.y;
    vertices[verticeIdx * 6 + 0].uv.y = (frameIdx.y + 1.0f) / (float)tileImages[imgIdx]->maxFrame.y;
    vertices[verticeIdx * 6 + 4].uv.y = (frameIdx.y + 1.0f) / (float)tileImages[imgIdx]->maxFrame.y;

    for (int i = 0; i < 6; i++)
    {
        vertices[verticeIdx * 6 + i].tileMapIdx = imgIdx;
        vertices[verticeIdx * 6 + i].color = color;
        vertices[verticeIdx * 6 + i].tileState = tileState;
        vertices[verticeIdx * 6 + i].tileRoomIdx = roomIdx;
        vertices[verticeIdx * 6 + i].tileDir = dir;
    }

    // SubResource - CPU와 GPU의 중간다리 역할
    D3D->GetDC()->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);
}

bool ObTileMap::WorldPosToTileIdx(Vector2 wpos, Int2& tileIdx)
{
    wpos -= GetWorldPos();

    Vector2 tileCoord;

    tileCoord.x = wpos.x / scale.x;
    tileCoord.y = wpos.y / scale.y;

    if ((tileCoord.x < 0) or (tileCoord.y < 0) or
        (tileCoord.x >= tileSize.x) or (tileCoord.y >= tileSize.y))
    {
        return false;
    }

    tileIdx.x = tileCoord.x;
    tileIdx.y = tileCoord.y;

    return true;
}

Vector2 ObTileMap::TileIdxToWorldPos(Int2 tileIdx)
{
    Vector2 Wpos;
    Wpos.x = tileIdx.x * scale.x;
    Wpos.y = tileIdx.y * scale.y;
    Wpos += GetWorldPos();
    return Wpos;
}

TileState ObTileMap::GetTileState(Int2 tileIdx)
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    return static_cast<TileState>(vertices[verticeIdx * 6].tileState);
}

void ObTileMap::SetTileState(Int2 tileIdx, TileState tileState)
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    vertices[verticeIdx * 6].tileState = static_cast<int>(tileState);
}

Vector2 ObTileMap::GetTilePosition(Int2 tileIdx)
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    return Vector2(vertices[verticeIdx * 6].position.x, vertices[verticeIdx * 6].position.y);
}

int ObTileMap::GetTileRoomIndex(Int2 tileIdx)
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    return vertices[verticeIdx * 6].tileRoomIdx;
}

void ObTileMap::SetTileRoomIndex(Int2 tileIdx, const int tileRoomIndex)
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    vertices[verticeIdx * 6].tileRoomIdx = tileRoomIndex;
}

DirState ObTileMap::GetTileDir(Int2 tileIdx)
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    return (DirState)vertices[verticeIdx * 6].tileDir;
}

void ObTileMap::SetTileDir(Int2 tileIdx, const DirState dir)
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    vertices[verticeIdx * 6].tileDir = dir;
}

void ObTileMap::Save()
{
    ofstream fout;
    string path = "../Contents/TileMap/" + file;

    //ios::out 쓰기 용으로 열겟다
    fout.open(path.c_str(), ios::out);

    //파일 열렷니?
    if (fout.is_open())
    {
        for (int i = 0; i < imageCount; i++)
        {
            fout << i << " ";

            if (tileImages[i])
            {
                string imgFile = "";
                imgFile.assign(tileImages[i]->file.begin(), tileImages[i]->file.end());

                fout << imgFile << " ";
                fout << tileImages[i]->maxFrame.x << " " << tileImages[i]->maxFrame.y << endl;
            }
            else
            {
                fout << "N" << endl;
            }
        }
    }

    fout << "TileSize " << tileSize.x << " " << tileSize.y << endl;
    fout << "TileScale " << scale.x << " " << scale.y << endl;
    fout << "TilePosition " << GetWorldPos().x << " " << GetWorldPos().y << endl;

    for (int i = 0; i < tileSize.y; i++)
    {
        //가로
        for (int j = 0; j < tileSize.x; j++)
        {
            int verticeIdx = tileSize.x * i + j;
            fout << j << " " << i << " "
                << vertices[verticeIdx * 6 + 1].uv.x << " " << vertices[verticeIdx * 6 + 1].uv.y << " "
                << vertices[verticeIdx * 6 + 2].uv.x << " " << vertices[verticeIdx * 6 + 2].uv.y << " "
                << vertices[verticeIdx * 6].color.x << " " << vertices[verticeIdx * 6].color.y << " "
                << vertices[verticeIdx * 6].color.z << " " << vertices[verticeIdx * 6].color.w << " "
                << vertices[verticeIdx * 6].tileMapIdx << " " << vertices[verticeIdx * 6].tileState << " "
                << vertices[verticeIdx * 6].tileRoomIdx << " " << vertices[verticeIdx * 6].tileDir << endl;
        }
    }

    //열엇으면 닫아라
    fout.close();
}

void ObTileMap::Load()
{
    ifstream fin;
    string path = "../Contents/TileMap/" + file;
    fin.open(path.c_str(), ios::in);

    string temp;

    if (fin.is_open())
    {
        for (int i = 0; i < imageCount; i++)
        {
            int idx; string Imgfile;
            fin >> idx;
            fin >> Imgfile;

            if (Imgfile != "N")
            {
                wstring wImgFile = L"";
                wImgFile.assign(Imgfile.begin(), Imgfile.end());

                tileImages[i] = make_shared<ObImage>(wImgFile);
                fin >> tileImages[i]->maxFrame.x >> tileImages[i]->maxFrame.y;
            }
        }
    }

    Int2 newTileSize;
    fin >> temp >> newTileSize.x >> newTileSize.y;

    ResizeTile(newTileSize);

    fin >> temp >> scale.x >> scale.y;

    Vector2 pos;
    fin >> temp >> pos.x >> pos.y;
    SetWorldPos(pos);

    for (int i = 0; i < tileSize.y; i++)
    {
        //가로
        for (int j = 0; j < tileSize.x; j++)
        {
            int temp; Vector2 minUV, maxUV; Color color; float tileMapIdx, tileMapState; int tileRoomIdx, tileDirState;

            fin >> temp >> temp >> minUV.x >> minUV.y >> maxUV.x >> maxUV.y
                >> color.x >> color.y >> color.z >> color.w >> tileMapIdx >> tileMapState
                >> tileRoomIdx >> tileDirState;

            int verticeIdx = tileSize.x * i + j;

            vertices[verticeIdx * 6].uv = Vector2(minUV.x, maxUV.y);
            vertices[verticeIdx * 6 + 1].uv = Vector2(minUV.x, minUV.y);
            vertices[verticeIdx * 6 + 2].uv = Vector2(maxUV.x, maxUV.y);
            vertices[verticeIdx * 6 + 3].uv = Vector2(maxUV.x, minUV.y);
            vertices[verticeIdx * 6 + 4].uv = Vector2(maxUV.x, maxUV.y);
            vertices[verticeIdx * 6 + 5].uv = Vector2(minUV.x, minUV.y);

            for (int k = 0; k < 6; k++)
            {
                vertices[verticeIdx * 6 + k].color = color;
                vertices[verticeIdx * 6 + k].tileMapIdx = tileMapIdx;
                vertices[verticeIdx * 6 + k].tileState = tileMapState;
                vertices[verticeIdx * 6 + k].tileRoomIdx = tileRoomIdx;
                vertices[verticeIdx * 6 + k].tileDir = tileDirState;
            }
        }

        CreateTileCost();
    }

    D3D->GetDC()->UpdateSubresource(vertexBuffer, 0, NULL, vertices, 0, 0);

    fin.close();
}

bool ObTileMap::isOnWall(const Int2 on)
{
    return GetTileState(on) == TileState::wall;
}

bool ObTileMap::isInTileState(const Vector2 wpos, const TileState tileState)
{
    Int2 on;
    if (WorldPosToTileIdx(wpos, on))
    {
        return GetTileState(on) == tileState;
    }
    return false;
}

bool ObTileMap::isFootOnWall(const ObRect* colTile)
{
    Vector2 pos;
    bool flag = false;
    
    pos = colTile->lt();
    flag |= isInTileState(pos, TileState::wall);
    pos = colTile->lb();
    flag |= isInTileState(pos, TileState::wall);
    pos = colTile->rt();
    flag |= isInTileState(pos, TileState::wall);
    pos = colTile->rb();
    flag |= isInTileState(pos, TileState::wall);

    return flag;
}

bool ObTileMap::isBodyOnPit(const ObRect* colTile)
{
    Vector2 pos;
    bool flag = true;

    pos = colTile->lt();
    flag &= (isInTileState(pos, TileState::pit) || isInTileState(pos, TileState::none));
    pos = colTile->lb();
    flag &= (isInTileState(pos, TileState::pit) || isInTileState(pos, TileState::none));
    pos = colTile->rt();
    flag &= (isInTileState(pos, TileState::pit) || isInTileState(pos, TileState::none));
    pos = colTile->rb();
    flag &= (isInTileState(pos, TileState::pit) || isInTileState(pos, TileState::none));

    return flag;
}

bool ObTileMap::PathFinding(Int2 sour, Int2 dest, OUT vector<Tile>& way, bool checkDiagnoal)
// bool ObTileMap::PathFinding(Int2 sour, Int2 dest, OUT vector<Tile*>& way, bool checkDiagnoal)
{
    //둘중에 하나가 벽이면 갈 수 있는길이 없다.
    //if (Tiles[dest.x][dest.y].state == TileState::wall)
    //{
    //    return false;
    //}

    //기존에 있던 길은 전부 비운다.
    way.clear();

    //출발지 목적지 같으면
    if (sour == dest)
    {
        return false;
    }

    //타일 비용 초기화
    for (int i = 0; i < tileSize.x; i++)
    {
        for (int j = 0; j < tileSize.y; j++)
        {
            Tiles[i][j].ClearCost();
        }
    }

    //우선순위 큐
    //            원소타입, 컨테이너타입, 비교함수
    priority_queue<PTile, vector<PTile>, ComparePTile> List;

    //리스트에 출발지를 추가
    Tile& tileRef = Tiles[sour.x][sour.y];
    tileRef.G = 0;          //출발지 현재비용은 0
    tileRef.ClacH(dest);    //목적지까지 예상비용 만들기
    tileRef.ClacF();        //총예상비용 만들기

    //          주소,  예상비용
    // List.push({ pTemp, pTemp->F });
    List.push({ tileRef, tileRef.F });

    //인접 타일 비용검사
    vector<Int2> LoopIdx;

    while (1)
    {
        //꺼낼 F값이 없다는 
        if (List.empty())
        {
            return false;
        }

        //탐색노드 받아오기
        PTile node = List.top();

        //탐색목록에서 제외
        node.first.isFind = true;

        //맨윗값 빼버리기
        List.pop();

        //맨윗값이 도착점이면 종료
        if (node.first.idx == dest)
        {
            break;
        }

        //왼쪽타일이 존재할때
        if (node.first.idx.x > 0)
        {
            Int2 on = Int2(node.first.idx.x - 1,
                node.first.idx.y);
            LoopIdx.push_back(on);
        }

        //위쪽타일이 존재할때
        if (node.first.idx.y > 0)
        {
            Int2 on = Int2(node.first.idx.x,
                node.first.idx.y - 1);
            LoopIdx.push_back(on);
        }

        //오른쪽타일이 존재할때
        if (node.first.idx.x < tileSize.x - 1)
        {
            Int2 on = Int2(node.first.idx.x + 1,
                node.first.idx.y);
            LoopIdx.push_back(on);
        }

        //아래쪽타일이 존재할때
        if (node.first.idx.y < tileSize.y - 1)
        {
            Int2 on = Int2(node.first.idx.x,
                node.first.idx.y + 1);
            LoopIdx.push_back(on);
        }

        if (checkDiagnoal)
        {
            //LB
            if (node.first.idx.x > 0 &&
                node.first.idx.y < tileSize.y - 1)
            {
                Int2 on = Int2(node.first.idx.x - 1,
                    node.first.idx.y + 1);

                // 주위 벽이 있을 땐 대각선으로 가지 않기
                TileState state1 = GetTileState(Int2(on.x - 1, on.y));
                TileState state2 = GetTileState(Int2(on.x, on.y - 1));
                if (state1 != TileState::wall &&
                    state2 != TileState::wall &&
                    state1 != TileState::pit &&
                    state2 != TileState::pit)
                {
                    LoopIdx.push_back(on);
                }
            }

            //LT
            if (node.first.idx.x > 0 &&
                node.first.idx.y > 0)
            {
                Int2 on = Int2(node.first.idx.x - 1,
                    node.first.idx.y - 1);

                TileState state1 = GetTileState(Int2(on.x - 1, on.y));
                TileState state2 = GetTileState(Int2(on.x, on.y + 1));
                if (state1 != TileState::wall &&
                    state2 != TileState::wall &&
                    state1 != TileState::pit &&
                    state2 != TileState::pit)
                {
                    LoopIdx.push_back(on);
                }
            }

            //RB
            if (node.first.idx.x < tileSize.x - 1 &&
                node.first.idx.y < tileSize.y - 1)
            {
                Int2 on = Int2(node.first.idx.x + 1,
                    node.first.idx.y + 1);

                TileState state1 = GetTileState(Int2(on.x + 1, on.y));
                TileState state2 = GetTileState(Int2(on.x, on.y - 1));
                if (state1 != TileState::wall &&
                    state2 != TileState::wall &&
                    state1 != TileState::pit &&
                    state2 != TileState::pit)
                {
                    LoopIdx.push_back(on);
                }
            }

            //RT
            if (node.first.idx.x < tileSize.x - 1 &&
                node.first.idx.y > 0)
            {
                Int2 on = Int2(node.first.idx.x + 1,
                    node.first.idx.y - 1);

                TileState state1 = GetTileState(Int2(on.x + 1, on.y));
                TileState state2 = GetTileState(Int2(on.x, on.y + 1));
                if (state1 != TileState::wall &&
                    state2 != TileState::wall &&
                    state1 != TileState::pit &&
                    state2 != TileState::pit)
                {
                    LoopIdx.push_back(on);
                }
            }
        }

        //상하좌우(대각선)타일 비용검사
        for (int i = 0; i < LoopIdx.size(); i++)
        {
            Tile& loopTile = Tiles[LoopIdx[i].x][LoopIdx[i].y];

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
                    {
                        List.push({ loopTile, loopTile.F });
                    }
                }
                break;
            }
        }
        LoopIdx.clear();
    }

    //도착지가 설정이 되었을때
    auto pTile = make_shared<Tile>(Tiles[dest.x][dest.y]);
    while (1)
    {
        way.emplace_back(*pTile);

        pTile = pTile->P;

        if (pTile == nullptr)
        {
            break;
        }
    }
    return true;
}

void Tile::ClearCost()
{
    F = G = H = INT_MAX;
    P = nullptr;
    isFind = false;
}

void Tile::ClacH(Int2 DestIdx)
{
    int tempX = abs(idx.x - DestIdx.x) * 10;
    int tempY = abs(idx.y - DestIdx.y) * 10;

    H = tempX + tempY;
}

void Tile::ClacF()
{
    F = G + H;
}