#include "stdafx.h"

namespace Gungeon
{
	Weapon3::Weapon3()
	{
		// 공통
		float scaleFactor = 1.0f;
		float effectScaleFactor = 1.0f;
		float uiWeaponScaleFactor = 2.0f;
		int uiBulletIdx = 0;

		// 차이
		type = WeaponType::machineGun;
		intervalFire = 0.05f;
		remainBulletCount = 300;

		scaleFactor = 2.0f;
		col->scale = Vector2(70.0f / 2.0f, 15.0f) * scaleFactor;

		idle = make_shared<ObImage>(L"EnterTheGungeon/Weapon/3/Weapon.png");
		idle->isVisible = true;
		idle->maxFrame.x = 2;
		idle->scale = Vector2(70.0f / 2.0f, 15.0f) * scaleFactor;
		idle->SetParentRT(*col);
		idle->pivot = OFFSET_LB;

		firePos->SetLocalPos(Vector2(col->scale.x / 2.0f, -10.0f));

		fireEffect->idle = make_shared<ObImage>(L"EnterTheGungeon/Weapon/3/Effect_Fire.png");
		fireEffect->idle->isVisible = false;
		fireEffect->idle->maxFrame.x = 3;
		fireEffect->idle->scale = Vector2(45.0f / 3.0f, 11.0f) * scaleFactor;
		fireEffect->idle->SetParentRT(*firePos);
		fireEffect->intervalDie = 0.2f;

		imgReloading = make_shared<ObImage>(L"EnterTheGungeon/Weapon/3/Reloading.png");
		imgReloading->isVisible = false;
		imgReloading->pivot = Vector2(0.4f, 0.25f);
		imgReloading->maxFrame.x = 2;
		imgReloading->scale = Vector2(42.0f / 2.0f, 22.0f) * scaleFactor;
		imgReloading->ChangeAnim(AnimState::loop, 0.1f);
		imgReloading->isVisible = false;
		imgReloading->SetParentRT(*col);

		pivotDefault = Vector2(0.5f, 0.25f);
		localPosDefault = Vector2(0.0f, -15.0f);
		localFirePosDefault = Vector2(70.0f, 5.0f);
		state = State::die;

		uiBulletFrame->img = make_shared<ObImage>(L"EnterTheGungeon/Weapon/3/UI_Magazine.png");
		uiBulletFrame->img->scale = Vector2(28.0f, 280.0f);
		uiBulletFrame->img->pivot = OFFSET_RB;
		uiBulletFrame->anchor = DirState::dirRB;
		uiBulletFrame->Spawn(Vector2(-26.0f, 30.0f));
		uiBulletFrame->img->space = Space::screen;
		uiBulletFrame->img->isVisible = false;

		bulletCount = 20;
		uiBullet.resize(bulletCount);

		uiBulletIdx = 0;
		for (auto& elem : uiBullet)
		{
			elem = make_shared<UI>();
			elem->img = make_shared<ObImage>(L"EnterTheGungeon/Weapon/UI_Bullet.png");
			elem->img->scale = Vector2(12.0f, 4.0f);
			elem->anchor = DirState::dirRB;
			elem->Spawn(Vector2(-40.0f, 56.0f + uiBulletIdx * 12.0f));
			elem->img->space = Space::screen;
			elem->img->isVisible = false;
			uiBulletIdx++;
		}

		uiWeaponScaleFactor = 2.5f;
		uiWeapon->img = make_shared<ObImage>(L"EnterTheGungeon/Weapon/3/UI_Weapon.png");
		uiWeapon->img->scale = Vector2(37.0f, 13.0f) * uiWeaponScaleFactor;
		uiWeapon->img->pivot = OFFSET_RB;
		uiWeapon->anchor = DirState::dirRB;
		uiWeaponSpawnPos = Vector2(-120.0f, 70.0f);
		uiWeapon->Spawn(uiWeaponSpawnPos);
		uiWeapon->img->space = Space::screen;
		uiWeapon->img->isVisible = false;
	}

	void Weapon3::Update()
	{
		Weapon::Update();
	}

	void Weapon3::Render()
	{
		Weapon::Render();
	}

	void Weapon3::ResizeScreen()
	{
		Weapon::ResizeScreen();
	}
}