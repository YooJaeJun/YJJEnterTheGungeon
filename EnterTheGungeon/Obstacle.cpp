#include "stdafx.h"

namespace Gungeon
{
	Obstacle::Obstacle()
	{
		col = make_shared<ObRect>();
		col->isVisible = false;
		col->isFilled = false;
		col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void Obstacle::Update()
	{
		Character::Update();
		idle->Update();
	}

	void Obstacle::LateUpdate()
	{
	}

	void Obstacle::Render()
	{
		idle->Render(); // RENDER->push(idle);
		Character::Render();
	}
}