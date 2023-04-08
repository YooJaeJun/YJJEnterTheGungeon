#include "stdafx.h"

namespace Gungeon
{
	Weapon::Weapon()
	{
		col = make_shared<ObRect>();
		col->isVisible = false;
		col->isFilled = false;
		col->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		col->pivot = OFFSET_LB;

		firePos = make_shared<ObRect>();
		firePos->isVisible = false;
		firePos->isFilled = false;
		firePos->scale = Vector2(10.0f, 10.0f);
		firePos->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		firePos->SetParentRT(*col);

		fireEffect = make_shared<Effect>();

		uiBulletFrame = make_shared<UI>();

		uiWeapon = make_shared<UI>();

		uiBulletCountInfinity = make_shared<UI>();
		uiBulletCountInfinity->img = make_shared<ObImage>(L"EnterTheGungeon/Weapon/UI_BulletCount_Infinity.png");
		uiBulletCountInfinity->img->isVisible = false;
		uiBulletCountInfinity->img->scale = Vector2(60.0f, 28.0f);
		uiBulletCountInfinity->img->pivot = OFFSET_RB;
		uiBulletCountInfinity->anchor = DirState::dirRB;
		uiBulletCountInfinityPos = Vector2(-70.0f, 155.0f);
		uiBulletCountInfinity->Spawn(uiBulletCountInfinityPos);
		uiBulletCountInfinity->img->space = Space::screen;
	}

	void Weapon::Update()
	{
		Character::Update();

		idle->Update();
		if (firePos) 
			firePos->Update();
		if (fireEffect) 
			fireEffect->Update();
		if (imgReloading) 
			imgReloading->Update();
		if (uiBulletFrame) 
			uiBulletFrame->Update();
		for (auto& elem : uiBullet) 
			if (elem) 
				elem->Update();
		if (uiWeapon) 
			uiWeapon->Update();
		if (uiBulletCountInfinity) 
			uiBulletCountInfinity->Update();
	}

	void Weapon::Render()
	{
		idle->Render(); // RENDER->push(idle);
		Character::Render();
		if (firePos) 
			firePos->Render();
		if (fireEffect) 
			fireEffect->Render();

		if (isUIRendering)
		{
			if (imgReloading) 
				imgReloading->Render();
			if (uiBulletFrame) 
				uiBulletFrame->Render();
			for (auto& elem : uiBullet) 
				if (elem) 
					elem->Render();
			if (uiWeapon) 
				uiWeapon->Render();
			if (uiBulletCountInfinity) 
				uiBulletCountInfinity->Render();
		}
	}

	void Weapon::ResizeScreen()
	{
		int idx = 0;

		uiBulletFrame->Spawn(Vector2(-26.0f, 30.0f));
		uiWeapon->Spawn(uiWeaponSpawnPos);
		if (remainBulletCount == INT_MAX)
			uiBulletCountInfinity->Spawn(uiBulletCountInfinityPos);

		idx = 0;
		for (auto& elem : uiBullet)
		{
			elem->Spawn(Vector2(-40.0f, 56.0f + idx * 12.0f));
			idx++;
		}
	}

	void Weapon::Equip()
	{
		col->SetLocalPos(localPosDefault);
		col->pivot = pivotDefault;
		idle->pivot = pivotDefault;
		firePos->SetLocalPos(localFirePosDefault);
		firePos->pivot = pivotDefault;
		if (fireEffect)
			fireEffect->idle->pivot = pivotDefault;
	}

	void Weapon::SwapUvY()
	{
		swap(idle->uv.y, idle->uv.w);
	}

	void Weapon::EquipRight()
	{
		SwapUvY();
		Equip();
	}

	void Weapon::EquipLeft()
	{
		SwapUvY();
		col->SetLocalPosX(-col->GetLocalPos().x);
		col->pivot.y = -pivotDefault.y;
		idle->pivot.y = -pivotDefault.y;
		firePos->SetLocalPosY(col->GetLocalPos().y);
		firePos->pivot.y = -pivotDefault.y;
		if (fireEffect)
			fireEffect->idle->pivot.y = -pivotDefault.y;
	}

	void Weapon::UIOn(const bool on)
	{
		isUIRendering = on;
	}
}