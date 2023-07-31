#include "stdafx.h"

namespace Gungeon
{
	Effect::Effect()
	{
		state = State::die;
	}

	void Effect::Update()
	{
		Character::Update();

		switch (state)
		{
		case Gungeon::State::idle:
			Idle();
			idle->Update();
			break;
		default: 
			break;
		}
	}

	void Effect::LateUpdate()
	{
	}

	void Effect::Render()
	{
		Character::Render();
		switch (state)
		{
		case Gungeon::State::idle:
			idle->Render();	// RENDER->push(idle);
			break;
		default:
			break;
		}
	}

	void Effect::Spawn(const Vector2 wpos)
	{
		state = State::idle;
		idle->SetWorldPos(wpos);
		idle->ChangeAnim(AnimState::once, 0.1f);
		idle->isVisible = true;
	}

	void Effect::Idle()
	{
		if (TIMER.GetTick(timeDie, intervalDie))
		{
			state = State::die;
			idle->SetWorldPos(DEFAULTSPAWN);
			idle->isVisible = false;
		}
	}
}