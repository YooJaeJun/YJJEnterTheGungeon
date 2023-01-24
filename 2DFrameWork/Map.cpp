#include "framework.h"

Map::Map()
{
    Init();
}

Map::~Map()
{
}

void Map::Init()
{
    // tilemap
    tilemap.reset(new ObTileMap);
    tilemap->scale = Vector2(100.0f, 100.0f);
    tilemap->SetWorldPos(Vector2(-5000.0f, -5000.0f));
    imgIdx = 1;
    tileSize = Int2(120, 120);
    tilemap->ResizeTile(tileSize);
    tileColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
    tileState = 0;
    tilemap->CreateTileCost();
    useGui = false;
}

void Map::Update()
{
    tilemap->Update();
    SetTilemapGUI();
}

void Map::Render()
{
    tilemap->Render();
}

void Map::SetTilemapGUI()
{
    if (false == useGui) return;


    // Gui
    if (ImGui::Button("ErrorFileSystem?->Click me"))
    {
        ImGuiFileDialog::Instance()->Close();
    }

    // TileScale
    ImGui::SliderFloat2("Scale", (float*)&tilemap->scale, 0.0f, 100.0f);

    //TileSize
    if (ImGui::SliderInt2("TileSize", (int*)&tileSize, 1, 100))
    {
        tilemap->ResizeTile(tileSize);
    }

    //TilePos
    Vector2 pos = tilemap->GetWorldPos();
    if (ImGui::SliderFloat2("TilePos", (float*)&pos, -5000.0f, 5000.0f))
    {
        tilemap->SetWorldPos(pos);
    }

    //TileState
    ImGui::SliderInt("TileState", &tileState, int(TileState::none), int(TileState::max));

    //TileColor
    ImGui::ColorEdit4("TileColor", (float*)&tileColor, ImGuiColorEditFlags_PickerHueWheel);

    //Texture
    for (int i = 0; i < tileImageCount; i++)
    {
        string str = "Texture" + to_string(i);
        if (GUI.FileImGui(str.c_str(), str.c_str(),
            ".jpg,.png,.bmp,.dds,.tga", "../Contents/Images/EnterTheGungeon"))
        {
            string path = ImGuiFileDialog::Instance()->GetCurrentFileName();
            wstring wImgFile = L"";
            wImgFile.assign(path.begin(), path.end());
            tilemap->tileImages[i].reset(new ObImage(wImgFile));
        }
        if (i < 3)
        {
            ImGui::SameLine();
        }
    }

    //ImageButton
    tilemap->RenderGui(pickingIdx, imgIdx);

    ImGui::Text("Picking Idx : %d , %d", pickingIdx.x, pickingIdx.y);
    ImGui::Text("Img Idx : %d", imgIdx);

    //maxFrame
    ImGui::InputInt2("Max Frame", (int*)&tilemap->tileImages[imgIdx]->maxFrame);

    //Coord
    ImGui::Text("Camera Pos : %f , %f", CAM.position.x, CAM.position.y);
    ImGui::Text("Camera Zoom Factor : %f", CAM.zoomFactor.x);

    ImGui::Text("Mouse Pos : %f , %f", INPUT.GetWorldMousePos().x, INPUT.GetWorldMousePos().y);

    if (tilemap->WorldPosToTileIdx(INPUT.GetWorldMousePos(), mouseIdx))
    {
        ImGui::Text("Mouse Idx : %d , %d", mouseIdx.x, mouseIdx.y);
        ImGui::Text("MouseOver TileState : %d", tilemap->Tiles[mouseIdx.x][mouseIdx.y].state);
        ImGui::Text("MouseOver RoomIndex : %d", tilemap->Tiles[mouseIdx.x][mouseIdx.y].roomIdx);
        ImGui::Text("MouseOver TileDir : %d", static_cast<int>(tilemap->Tiles[mouseIdx.x][mouseIdx.y].dir));
    }
    else
    {
        ImGui::Text("Mouse Idx : NoTileZone");
        ImGui::Text("MouseOver TileState : NoTileZone");
        ImGui::Text("MouseOver RoomIndex : NoTileZone");
        ImGui::Text("MouseOver TileDir : NoTileZone");
    }

    //SaveLoad
    if (GUI.FileImGui("Save", "Save Map",
        ".txt", "../Contents/TileMap"))
    {
        string path = ImGuiFileDialog::Instance()->GetCurrentFileName();
        tilemap->file = path;
        tilemap->Save();
    }
    ImGui::SameLine();
    if (GUI.FileImGui("Load", "Load Map",
        ".txt", "../Contents/TileMap"))
    {
        string path = ImGuiFileDialog::Instance()->GetCurrentFileName();
        tilemap->file = path;
        tilemap->Load();
        tileSize = tilemap->GetTileSize();
    }

    //Brush
    ImVec2 min = ImGui::GetWindowPos();
    ImVec2 max;
    max.x = min.x + ImGui::GetWindowSize().x;
    max.y = min.y + ImGui::GetWindowSize().y;

    if (!ImGui::IsMouseHoveringRect(min, max, false) &&
        !ImGuiFileDialog::Instance()->IsOpened())
    {
        if (INPUT.KeyPress(VK_LBUTTON))
        {
            if (tilemap->WorldPosToTileIdx(INPUT.GetWorldMousePos(), mouseIdx))
            {
                tilemap->SetTile(mouseIdx, pickingIdx, imgIdx, tileState, tileColor);
            }
        }
    }

    tilemap->Update();
}
