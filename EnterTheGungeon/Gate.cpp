#include "stdafx.h"

namespace Gungeon
{
    Gate::Gate()
    {
        Init();
    }

    void Gate::Init()
    {
        float scaleFactor = 2.0f;

        col->scale = Vector2(70.0f, 70.0f) * scaleFactor;
        col->pivot = OFFSET_B;
        SetPos(DEFAULTSPAWN);

        colTile = make_shared<ObRect>();
        colTile->isVisible = false;
        colTile->scale = Vector2(40.0f, 20.0f) * scaleFactor;
        colTile->color = Color(1.0f, 1.0f, 1.0f);
        colTile->isFilled = false;
        colTile->SetParentRT(*col);
        colTile->SetLocalPosY(-20.0f);
        colTile->pivot = OFFSET_B;

        idle = make_shared<ObImage>(L"EnterTheGungeon/Level/Gate.png");
        idle->isVisible = false;
        idle->maxFrame.x = 15;
        idle->scale = Vector2(960.0f / 15.0f, 165.0f) * scaleFactor;
        idle->SetParentRT(*col);
        idle->SetLocalPosY(-20.0f);
        idle->pivot = OFFSET_B;

        bottom = make_shared<ObImage>(L"EnterTheGungeon/Level/Gate_Bottom.png");
        bottom->isVisible = false;
        bottom->scale = Vector2(64.0f, 73.0f) * scaleFactor;
        bottom->SetParentRT(*col);
        bottom->SetLocalPosY(-20.0f);
        bottom->pivot = OFFSET_B;

        gateState = GateState::none;
    }

    void Gate::Update()
    {
        switch (gateState)
        {
        case Gungeon::GateState::none:
            break;

        case Gungeon::GateState::opening:
            if (TIMER.GetTick(timeOpen, 1.5f))
                gateState = GateState::open;
            break;

        case Gungeon::GateState::open:
            if (flagIntersectPlayer)
                gateState = GateState::cinematic;
            break;

        case Gungeon::GateState::cinematic:

            if (TIMER.GetTick(timeCinematicBox, 1.0f))
                gateState = GateState::process;
            break;

        case Gungeon::GateState::process:
            break;

        case Gungeon::GateState::setting:
            if (TIMER.GetTick(timeSet, 0.5f))
                gateState = GateState::set;
            break;

        case Gungeon::GateState::set:
            idle->ChangeAnim(AnimState::reverseOnce, 0.1f);
            gateState = GateState::closing;
            break;

        case Gungeon::GateState::closing:

            if (TIMER.GetTick(timePlayerDisappear, 0.2f))
                flagPlayerDisappear = true;

            if (TIMER.GetTick(timeClosed, 0.3f))
            {
                gateState = GateState::closed;
                SOUND.Play("GateSlam");
            }
            break;

        case Gungeon::GateState::closed:
            break;
        }


        Obstacle::Update();
        col->Update();
        colTile->Update();
        bottom->Update();
    }

    void Gate::LateUpdate()
    {
    }

    void Gate::Render()
    {
        bottom->Render();
        Obstacle::Render();
        col->Render();
        colTile->Render();
    }

    void Gate::Spawn(const Vector2 wpos)
    {
        SetPos(wpos);
        idle->ChangeAnim(AnimState::once, 0.15f);
        idle->isVisible = true;
        bottom->isVisible = true;
        gateState = GateState::opening;
        SOUND.Play("GateSlam");
    }
}