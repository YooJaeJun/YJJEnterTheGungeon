#include "framework.h"

ObTileMap::ObTileMap()
{
    vertexBuffer.Reset();;
    vertices.reset();

    //타일 가로 세로 개수
    tileSize.x = 20;
    tileSize.y = 20;

    file = "EnterTheGungeon.txt";
    //하나의 타일 크기
    scale = Vector2(50.0f, 50.0f);

    tileImages[0] = make_shared<ObImage>(L"Empty.png");
    tileImages[0]->maxFrame = Vec2i(10, 10);

    tileImages[1] = make_shared<ObImage>(L"EnterTheGungeon/Level/Tileset.png");
    tileImages[1]->maxFrame = Vec2i(10, 10);

    tileImages[2] = make_shared<ObImage>(L"Tile.png");
    tileImages[2]->maxFrame = Vec2i(8, 6);

    tileImages[3] = make_shared<ObImage>(L"Tile2.png");
    tileImages[3]->maxFrame = Vec2i(11, 7);

    ObTileMap::ResizeTile(tileSize);
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

    const Vector2 half = scale * 0.5f;
    Update();
    for (int i = 0; i < tileSize.x; i++)
    {
        for (int j = 0; j < tileSize.y; j++)
        {
            Tiles[i][j].idx = Vec2i(i, j);
            Tiles[i][j].state = GetTileState(Tiles[i][j].idx);
            Tiles[i][j].roomIdx = GetTileRoomIndex(Tiles[i][j].idx);
            Tiles[i][j].dir = GetTileDir(Tiles[i][j].idx);

            Tiles[i][j].Pos.x = i * scale.x + GetWorldPos().x + half.x;
            Tiles[i][j].Pos.y = j * scale.y + GetWorldPos().y + half.y;
        }
    }
}

//사이즈 재조정, 정정 재조정
void ObTileMap::ResizeTile(Vec2i newTileSize)
{
	const shared_ptr<VertexTile[]> newVertices { new VertexTile[newTileSize.x * newTileSize.y * 6] };

    //세로
    for (int i = 0; i < newTileSize.y; i++)
    {
        //가로
        for (int j = 0; j < newTileSize.x; j++)
        {
	        const int tileIdx = newTileSize.x * i + j;
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
	    const Vec2i minV = Vec2i(min(newTileSize.x, tileSize.x), min(newTileSize.y, tileSize.y));
        for (int i = 0; i < minV.y; i++)
        {
            for (int j = 0; j < minV.x; j++)
            {
	            const int SrcIdx = tileSize.x * i + j;
	            const int DestIdx = newTileSize.x * i + j;

                for (int k = 0; k < 6; k++)
                    newVertices[DestIdx * 6 + k] = vertices[SrcIdx * 6 + k];
            }
        }
    }

    vertices.reset();
    vertices = newVertices;
    vertexBuffer.Reset();
    tileSize = newTileSize;
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(VertexTile) * tileSize.x * tileSize.y * 6;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices.get();
        const HRESULT hr = D3D.GetDevice()->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
        Check(hr);
    }
}

void ObTileMap::RenderGui(Vec2i& guiPickingIdx, int& imgIdx) const
{
    if (ImGui::InputInt("ImgIdx", &imgIdx))
    {
        if (imgIdx > tileImageCount - 1)
            imgIdx = 0;
        else if (imgIdx < 0)
            imgIdx = tileImageCount - 1;
    }

    const Vec2i MF = tileImages[imgIdx]->maxFrame;
    ImVec2 size;
    size.x = 200.0f / static_cast<float>(MF.x);
    size.y = 200.0f / static_cast<float>(MF.y);

    //텍스쳐 좌표
    int index = 0;

    for (UINT i = 0; i < static_cast<UINT>(MF.y); i++)
    {
        for (UINT j = 0; j < static_cast<UINT>(MF.x); j++)
        {
	        ImVec2 LT, RB;

	        if (j != 0)
                ImGui::SameLine();

            //텍스쳐 좌표
            LT.x = 1.0f / MF.x * j;
            LT.y = 1.0f / MF.y * i;
            RB.x = 1.0f / MF.x * (j + 1);
            RB.y = 1.0f / MF.y * (i + 1);

            ImGui::PushID(index);
            if (ImGui::ImageButton((void*)tileImages[imgIdx]->SRV.Get(), size, LT, RB))
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

    for (int i = 0; i < tileImageCount; i++)
    {
        if (tileImages[i])
        {
            D3D.GetDC()->PSSetShaderResources(i, 1, tileImages[i]->SRV.GetAddressOf());
            D3D.GetDC()->PSSetSamplers(i, 1, tileImages[i]->sampler.GetAddressOf());
        }
    }

    tileMapShader->Set();

    constexpr UINT stride = sizeof(VertexTile);
    constexpr UINT offset = 0;

    //버텍스버퍼 바인딩
    D3D.GetDC()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    D3D.GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    D3D.GetDC()->Draw(tileSize.x * tileSize.y * 6, 0);
}

void ObTileMap::SetTile(
    const Vec2i tileIdx, 
    const Vec2i frameIdx, 
    const int imgIdx, 
    const int tileState,
    const Color color,
    const int roomIdx,
    const DirState dir) const
{
    int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;

    vertices[verticeIdx * 6 + 0].uv.x = frameIdx.x / static_cast<float>(tileImages[imgIdx]->maxFrame.x);
    vertices[verticeIdx * 6 + 1].uv.x = frameIdx.x / static_cast<float>(tileImages[imgIdx]->maxFrame.x);
    vertices[verticeIdx * 6 + 5].uv.x = frameIdx.x / static_cast<float>(tileImages[imgIdx]->maxFrame.x);

    vertices[verticeIdx * 6 + 2].uv.x = (frameIdx.x + 1.0f) / static_cast<float>(tileImages[imgIdx]->maxFrame.x);
    vertices[verticeIdx * 6 + 3].uv.x = (frameIdx.x + 1.0f) / static_cast<float>(tileImages[imgIdx]->maxFrame.x);
    vertices[verticeIdx * 6 + 4].uv.x = (frameIdx.x + 1.0f) / static_cast<float>(tileImages[imgIdx]->maxFrame.x);

    vertices[verticeIdx * 6 + 3].uv.y = frameIdx.y / static_cast<float>(tileImages[imgIdx]->maxFrame.y);
    vertices[verticeIdx * 6 + 1].uv.y = frameIdx.y / static_cast<float>(tileImages[imgIdx]->maxFrame.y);
    vertices[verticeIdx * 6 + 5].uv.y = frameIdx.y / static_cast<float>(tileImages[imgIdx]->maxFrame.y);

    vertices[verticeIdx * 6 + 2].uv.y = (frameIdx.y + 1.0f) / static_cast<float>(tileImages[imgIdx]->maxFrame.y);
    vertices[verticeIdx * 6 + 0].uv.y = (frameIdx.y + 1.0f) / static_cast<float>(tileImages[imgIdx]->maxFrame.y);
    vertices[verticeIdx * 6 + 4].uv.y = (frameIdx.y + 1.0f) / static_cast<float>(tileImages[imgIdx]->maxFrame.y);

    for (int i = 0; i < 6; i++)
    {
        vertices[verticeIdx * 6 + i].tileMapIdx = imgIdx;
        vertices[verticeIdx * 6 + i].color = color;
        vertices[verticeIdx * 6 + i].tileState = tileState;
        vertices[verticeIdx * 6 + i].tileRoomIdx = roomIdx;
        vertices[verticeIdx * 6 + i].tileDir = dir;
    }

    // SubResource - CPU와 GPU의 중간다리 역할
    D3D.GetDC()->UpdateSubresource(
        vertexBuffer.Get(), 
        0, 
        nullptr, 
        vertices.get(), 
        0, 0);
}

bool ObTileMap::WorldPosToTileIdx(Vector2 wpos, Vec2i& tileIdx)
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

    tileIdx.x = static_cast<int>(tileCoord.x);
    tileIdx.y = static_cast<int>(tileCoord.y);

    return true;
}

Vector2 ObTileMap::TileIdxToWorldPos(const Vec2i tileIdx) const
{
    Vector2 Wpos;
    Wpos.x = tileIdx.x * scale.x;
    Wpos.y = tileIdx.y * scale.y;
    Wpos += GetWorldPos();
    return Wpos;
}

TileState ObTileMap::GetTileState(const Vec2i tileIdx) const
{
    const int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    return static_cast<TileState>(vertices[verticeIdx * 6].tileState);
}

void ObTileMap::SetTileState(const Vec2i tileIdx, TileState tileState) const
{
	const int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    vertices[verticeIdx * 6].tileState = static_cast<int>(tileState);
}

Vector2 ObTileMap::GetTilePosition(const Vec2i tileIdx) const
{
	const int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    return Vector2(vertices[verticeIdx * 6].position.x, vertices[verticeIdx * 6].position.y);
}

int ObTileMap::GetTileRoomIndex(const Vec2i tileIdx) const
{
	const int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    return vertices[verticeIdx * 6].tileRoomIdx;
}

void ObTileMap::SetTileRoomIndex(const Vec2i tileIdx, const int tileRoomIndex) const
{
	const int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    vertices[verticeIdx * 6].tileRoomIdx = tileRoomIndex;
}

DirState ObTileMap::GetTileDir(const Vec2i tileIdx) const
{
	const int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    return static_cast<DirState>(vertices[verticeIdx * 6].tileDir);
}

void ObTileMap::SetTileDir(Vec2i tileIdx, const DirState dir) const
{
	const int verticeIdx = tileSize.x * tileIdx.y + tileIdx.x;
    vertices[verticeIdx * 6].tileDir = dir;
}

void ObTileMap::Save() const
{
    const string path = "../Contents/TileMap/" + file;
    ofstream fout;

    //ios::out 쓰기 용으로 열겟다
    fout.open(path.c_str(), ios::out);

    if (fout.is_open())
    {
        for (int i = 0; i < tileImageCount; i++)
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
	        const int verticeIdx = tileSize.x * i + j;
            fout << j << " " << i << " "
                << vertices[verticeIdx * 6 + 1].uv.x << " " << vertices[verticeIdx * 6 + 1].uv.y << " "
                << vertices[verticeIdx * 6 + 2].uv.x << " " << vertices[verticeIdx * 6 + 2].uv.y << " "
                << vertices[verticeIdx * 6].color.x << " " << vertices[verticeIdx * 6].color.y << " "
                << vertices[verticeIdx * 6].color.z << " " << vertices[verticeIdx * 6].color.w << " "
                << vertices[verticeIdx * 6].tileMapIdx << " " << vertices[verticeIdx * 6].tileState << " "
                << vertices[verticeIdx * 6].tileRoomIdx << " " << vertices[verticeIdx * 6].tileDir << endl;
        }
    }

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
        for (int i = 0; i < tileImageCount; i++)
        {
            int idx;
        	string Imgfile;
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

    Vec2i newTileSize;
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
            int temp; 
            Vector2 minUV, maxUV; 
            Color color; 
            int tileMapIdx, tileMapState; 
            int tileRoomIdx, tileDirState;

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

    D3D.GetDC()->UpdateSubresource(
        vertexBuffer.Get(), 
        0, 
        nullptr, 
        vertices.get(), 
        0, 0);

    fin.close();
}

bool ObTileMap::isOnWall(const Vec2i on) const
{
    return GetTileState(on) == TileState::wall;
}

bool ObTileMap::isInTileState(const Vector2 wpos, const TileState tileState)
{
    Vec2i on;
    if (WorldPosToTileIdx(wpos, on))
    {
        return GetTileState(on) == tileState;
    }
    return false;
}

bool ObTileMap::isFootOnWall(const shared_ptr<ObRect> colTile)
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

bool ObTileMap::isBodyOnPit(const shared_ptr<ObRect> colTile)
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