#include "stdafx.h"

namespace Gungeon
{
    UI::UI()
    {
    }

    void UI::Update()
    {
        if (img) 
            img->Update();
    }

    void UI::LateUpdate()
    {
    }

    void UI::Render()
    {
        if (img) 
            img->Render();
    }

    void UI::Spawn(const Vector2 factor)
    {
        switch (anchor)
        {
        case DirState::dirB:
            img->SetWorldPos(Vector2(factor.x, -app.GetHalfHeight() + factor.y));
            break;
        case DirState::dirL:
            img->SetWorldPos(Vector2(-app.GetHalfWidth() + factor.x, factor.y));
            break;
        case DirState::dirR:
            img->SetWorldPos(Vector2(app.GetHalfWidth() + factor.x, factor.y));
            break;
        case DirState::dirLB:
            img->SetWorldPos(Vector2(-app.GetHalfWidth() + factor.x, -app.GetHalfHeight() + factor.y));
            break;
        case DirState::dirRB:
            img->SetWorldPos(Vector2(app.GetHalfWidth() + factor.x, -app.GetHalfHeight() + factor.y));
            break;
        case DirState::dirT:
            img->SetWorldPos(Vector2(factor.x, app.GetHalfHeight() + factor.y));
            break;
        case DirState::dirLT:
            img->SetWorldPos(Vector2(-app.GetHalfWidth() + factor.x, app.GetHalfHeight() + factor.y));
            break;
        case DirState::dirRT:
            img->SetWorldPos(Vector2(app.GetHalfWidth() + factor.x, app.GetHalfHeight() + factor.y));
            break;
        default:
            img->SetWorldPos(Vector2(factor.x, factor.y));
            break;
        }
    }

    bool UI::DownGauge()
    {
        if (img->scale.x <= 0.0f)
        {
            img->scale.x = 0.0f;
            img->uv.z = 0.0f;
            return false;
        }
        else
        {
            img->scale.x -= imgSize.x / 50.0f * DELTA;
            img->uv.z = img->scale.x / imgSize.x;
        }
        return true;
    }
}