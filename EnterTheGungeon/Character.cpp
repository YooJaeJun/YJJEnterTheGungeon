#include "stdafx.h"

namespace Gungeon
{
	Character::Character()
	{
	}

	void Character::Init()
	{
	}

	void Character::Release()
	{
		SafeDelete(col);
		SafeDelete(colTile);
	}

	void Character::Update()
	{
		if (col)
		{
			col->Update();
			MAP->tilemap->WorldPosToTileIdx(Pos(), on);
		}
		if (colTile) colTile->Update();
	}

	void Character::Update(const bool notRotation)
	{
		if (col) col->Update(notRotation);
		if (colTile) colTile->Update(notRotation);
	}

	void Character::LateUpdate()
	{
	}

	void Character::Render()
	{
		if (col) col->Render(); // RENDER->push(col);
		if (colTile) colTile->Render();	// RENDER->push(colTile);
	}

	void Character::ResizeScreen()
	{
	}

	void Character::Spawn(const Vector2 wpos)
	{
		SetPos(wpos);
		col->Update();

		state = State::idle;
		isHit = false;
	}

	void Character::StepBack()
	{
		SetPos(lastPos);
	}

	void Character::SetLastPos()
	{
		lastPos = Pos();
	}

	void Character::ColToggle()
	{
		if (col) col->isVisible ^= 1;
		if (colTile) colTile->isVisible ^= 1;
	}
}