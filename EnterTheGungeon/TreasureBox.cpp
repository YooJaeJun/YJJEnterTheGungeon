#include "stdafx.h"

namespace Gungeon
{
    TreasureBox::TreasureBox()
    {
        Init();
    }

    void TreasureBox::Init()
    {
        col->scale = Vector2(50.0f / 2.0f, 19.0f) * 3.0f;
        col->SetWorldPos(DEFAULTSPAWN);
        col->isVisible = false;

        idle = make_shared<ObImage>(L"EnterTheGungeon/Level/Treasure.png");
        idle->maxFrame.x = 5;
        idle->scale = Vector2(135.0f / 5.0f, 26.0f) * 3.0f;
        idle->ChangeAnim(AnimState::loop, 0.2f);
        idle->SetParentT(*col);

        open = make_shared<ObImage>(L"EnterTheGungeon/Level/Treasure_Open.png");
        open->isVisible = false;
        open->maxFrame.x = 2;
        open->scale = Vector2(54.0f / 2.0f, 26.0f) * 3.0f;
        open->SetParentT(*col);

        for (auto& elem : weapon) elem = nullptr;
    }

    void TreasureBox::Update()
    {
        Obstacle::Update();

        int weaponIdx = 0;
        switch (treasureState)
        {
        case Gungeon::TreasureState::none:
        case Gungeon::TreasureState::spawn:
            break;
        case Gungeon::TreasureState::opening:
            idle->isVisible = false;
            open->isVisible = true;
            open->ChangeAnim(AnimState::once, 0.2f);

            weaponIdx = 0;
            for (auto& elem : weapon)
            {
                if (weaponIdx == 0) elem = make_shared<Weapon2>();
                else if (weaponIdx == 1) elem = make_shared<Weapon3>();
                elem->Spawn(Vector2(Pos().x, Pos().y));
                elem->idle->isVisible = true;
                weaponIdx++;
            }

            treasureState = TreasureState::dropping;
            break;

        case Gungeon::TreasureState::dropping:
            weaponIdx = 0;
            for (auto& elem : weapon)
            {
                if (weaponIdx == 0) weaponDest[weaponIdx] = Vector2(Pos().x + 100.0f, Pos().y);
                else if (weaponIdx == 1) weaponDest[weaponIdx] = Vector2(Pos().x - 150.0f, Pos().y);
                Vector2 velocity = weaponDest[weaponIdx] - Pos();
                weapon[weaponIdx]->col->MoveWorldPos(velocity * 3.0f * DELTA);

                if (abs(weapon[weaponIdx]->Pos().x - weaponDest[weaponIdx].x) < 10.0f)
                {
                    treasureState = TreasureState::finish;
                }

                weapon[weaponIdx]->Update();

                weaponIdx++;
            }
            break;

        case Gungeon::TreasureState::finish:
            weaponIdx = 0;
            for (auto& elem : weapon)
            {
                weapon[weaponIdx]->Update();
                weaponIdx++;
            }
            break;
        }

        col->Update();
        open->Update();
    }

    void TreasureBox::Render()
    {
        Obstacle::Render();
        open->Render();
        col->Render();

        switch (treasureState)
        {
        case Gungeon::TreasureState::none:
        case Gungeon::TreasureState::spawn:
        case Gungeon::TreasureState::opening:
            break;
        case Gungeon::TreasureState::dropping:
        case Gungeon::TreasureState::finish:
            int weaponIdx = 0;
            for (auto& elem : weapon)
            {
                weapon[weaponIdx]->Render();
                weaponIdx++;
            }
            break;
        }
    }

    void TreasureBox::Spawn(const Vector2 wpos)
    {
        SetPos(wpos);
        treasureState = TreasureState::spawn;
        SOUND.Play("ChestUnlock");
    }
}