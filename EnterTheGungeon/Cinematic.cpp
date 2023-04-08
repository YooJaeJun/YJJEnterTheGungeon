#include "stdafx.h"

namespace Gungeon
{
    Cinematic::Cinematic()
    {
        Init();
    }

    void Cinematic::Init()
    {
        for (auto& elem : box)
        {
            elem = make_shared<UI>();
            elem->img = make_shared<ObImage>(L"EnterTheGungeon/Level/Cinematic_Box.png");
            elem->img->isVisible = false;
            elem->img->scale = Vector2(app.GetWidth(), 150.0f);
            elem->img->isFilled = true;
            elem->img->color = Color(0.0f, 0.0f, 0.0f);
            elem->img->space = Space::screen;
            elem->img->pivot = OFFSET_LT;
        }
        box[0]->anchor = DirState::dirLT;
        box[0]->Spawn(Vector2(0.0f, 150.0f));
        box[1]->anchor = DirState::dirLB;
        box[1]->Spawn(Vector2(0.0f, 0.0f));

        cinematicState = CinematicState::none;

        for (auto& elem : timeCinematic)
            elem = 0.0f;

        intervalCinematic[(int)CinematicState::cinematicBox1] = 1.0f;
        intervalCinematic[(int)CinematicState::cinematicBox2] = 1.0f;
        intervalCinematic[(int)CinematicState::cutScene] = 3.0f;
        intervalCinematic[(int)CinematicState::cinematicBox3] = 1.0f;
        intervalCinematic[(int)CinematicState::cinematicBox4] = 1.0f;
    }

    void Cinematic::Update()
    {
        switch (cinematicState)
        {
            // 등장연출
        case Gungeon::CinematicState::none:
            break;

        case Gungeon::CinematicState::cinematicBox1:
            box[0]->img->isVisible = true;
            box[1]->img->isVisible = true;
            BoxUp(true);

            if (TIMER.GetTick(timeCinematic[(int)CinematicState::cinematicBox1],
                intervalCinematic[(int)CinematicState::cinematicBox1]))
                cinematicState = CinematicState::cameraTargeting1;
            break;

        case Gungeon::CinematicState::cameraTargeting1:
            break;

        case Gungeon::CinematicState::bossSpawnAnim:
            break;

        case Gungeon::CinematicState::cutScene:
            if (TIMER.GetTick(timeCinematic[(int)CinematicState::cutScene],
                intervalCinematic[(int)CinematicState::cutScene]))
                cinematicState = CinematicState::cameraTargeting2;
            break;

        case Gungeon::CinematicState::cameraTargeting2:
            break;

        case Gungeon::CinematicState::cinematicBox2:
            BoxUp(false);

            if (TIMER.GetTick(timeCinematic[(int)CinematicState::cinematicBox2],
                intervalCinematic[(int)CinematicState::cinematicBox2]))
                cinematicState = CinematicState::finish;
            break;

        case Gungeon::CinematicState::finish:
            break;

            // 사망연출
        case Gungeon::CinematicState::cinematicBox3:
            box[0]->img->isVisible = true;
            box[1]->img->isVisible = true;
            BoxUp(true);

            if (TIMER.GetTick(timeCinematic[(int)CinematicState::cinematicBox3],
                intervalCinematic[(int)CinematicState::cinematicBox3]))
                cinematicState = CinematicState::cameraTargeting3;
            break;

        case Gungeon::CinematicState::cameraTargeting3:
            break;

        case Gungeon::CinematicState::bossDieAnim:
            break;

        case Gungeon::CinematicState::cameraTargeting4:
        case Gungeon::CinematicState::cinematicBox4:
            BoxUp(false);

            if (TIMER.GetTick(timeCinematic[(int)CinematicState::cinematicBox4],
                intervalCinematic[(int)CinematicState::cinematicBox4]))
            {
                cinematicState = CinematicState::finish2;
                SOUND.Play("Win");
            }

        case Gungeon::CinematicState::finish2:
            break;

        case Gungeon::CinematicState::clear:
            break;
        }

        for (auto& elem : box) 
            elem->Update();
    }

    void Cinematic::Render()
    {
        for (auto& elem : box) 
            elem->Render();
    }

    void Cinematic::ResizeScreen()
    {
        box[0]->Spawn(Vector2(0.0f, 150.0f));
        box[1]->Spawn(Vector2(0.0f, 0.0f));
    }

    void Cinematic::BoxUp(const bool isUp)
    {
        float factor = 75.0f;
        if (false == isUp) 
            factor *= -1;

        box[0]->img->MoveWorldPos(Vector2(0.0f, -factor * DELTA));
        box[1]->img->MoveWorldPos(Vector2(0.0f, factor * DELTA));
    }
}