#include "stdafx.h"

namespace Gungeon
{
    void Scene01::Init()
    {
        CAM.position = Vector2(0.0f, 0.0f);
        CAM.zoomFactor = Vector3(0.08f, 0.08f, 0.08f);

        if (mapGen && mapGen->selectedRooms.size() > 0)
        {
	        int idx = 0;
	        for (const auto& elem : mapGen->selectedRooms)
            {
                if (idx == 1)
                    elem->treasureSpawner->isVisible = true;
                for (const auto& enemySpawerElem : elem->enemySpawner)
                    enemySpawerElem->isVisible = true;
                idx++;
            }
        }

        SOUND.Play("BGM_MapGen");
    }

    void Scene01::Update()
    {
        if (ImGui::Button("Current Scene"))
            ChangeScene1();
        ImGui::SameLine();
        if (ImGui::Button("Next Scene"))
            ChangeScene2();


        if (INPUT.KeyPress(VK_LEFT) || INPUT.KeyPress('A'))
            CAM.position.x -= 1500.0f * DELTA;
        if (INPUT.KeyPress(VK_RIGHT) || INPUT.KeyPress('D'))
            CAM.position.x += 1500.0f * DELTA;
        if (INPUT.KeyPress(VK_UP) || INPUT.KeyPress('W'))
            CAM.position.y += 1500.0f * DELTA;
        if (INPUT.KeyPress(VK_DOWN) || INPUT.KeyPress('S'))
            CAM.position.y -= 1500.0f * DELTA;


        if (mapGen) 
            mapGen->Update();

        ChangeUpdateScene();

        SOUND.SetVolume("BGM_MapGen", 0.6f);
    }

    void Scene01::Render()
    {
        if (mapGen) 
            mapGen->Render();
    }

    void Scene01::ChangeScene1() const
    {
        MAPINFO.Init();
        mapGen->Init();
    }

    void Scene01::ChangeScene2()
    {
        isChangingScene = true;
        {
	        const shared_ptr<Scene02> tempScene = make_shared<Scene02>();
            tempScene->mapGen = mapGen;
            tempScene->player = player;
            SCENE.AddScene("Scene02", tempScene);
        }
        SCENE.ChangeScene("Scene02", 1.0f);
    }

    void Scene01::ChangeUpdateScene()
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
}