#include "stdafx.h"

namespace Gungeon
{
	Weapon2::Weapon2()
	{
		// Â÷ÀÌ
		type = WeaponType::shotgun;
		intervalFire = 0.7f;
		remainBulletCount = 150;

		constexpr float scaleFactor = 2.0f;
		col->scale = Vector2(84.0f / 2.0f, 23.0f) * scaleFactor;

		idle = make_shared<ObImage>(L"EnterTheGungeon/Weapon/2/Weapon.png");
		idle->isVisible = true;
		idle->maxFrame.x = 2;
		idle->scale = Vector2(84.0f / 2.0f, 23.0f) * scaleFactor;
		idle->SetParentRT(*col);
		idle->pivot = OFFSET_LB;

		firePos->SetLocalPos(Vector2(col->scale.x / 2.0f, 0.0f));

		constexpr float effectScaleFactor = 1.0f;
		fireEffect->idle = make_shared<ObImage>(L"EnterTheGungeon/Weapon/2/Effect_Fire.png");
		fireEffect->idle->isVisible = false;
		fireEffect->idle->maxFrame.x = 3;
		fireEffect->idle->scale = Vector2(45.0f / 3.0f, 11.0f) * effectScaleFactor;
		fireEffect->idle->SetParentRT(*firePos);
		fireEffect->intervalDie = 0.2f;

		imgReloading = make_shared<ObImage>(L"EnterTheGungeon/Weapon/2/Reloading.png");
		imgReloading->isVisible = false;
		imgReloading->pivot = Vector2(0.4f, 0.25f);
		imgReloading->maxFrame.x = 2;
		imgReloading->scale = Vector2(42.0f / 2.0f, 22.0f) * scaleFactor;
		imgReloading->ChangeAnim(AnimState::loop, 0.1f);
		imgReloading->isVisible = false;
		imgReloading->SetParentRT(*col);

		pivotDefault = Vector2(0.6f, 0.25f);
		localPosDefault = Vector2(-10.0f, -15.0f);
		localFirePosDefault = Vector2(80.0f, 6.0f);
		state = State::die;

		uiBulletFrame->img = make_shared<ObImage>(L"EnterTheGungeon/Weapon/2/UI_Magazine.png");
		uiBulletFrame->img->scale = Vector2(28.0f, 124.0f);
		uiBulletFrame->img->pivot = OFFSET_RB;
		uiBulletFrame->anchor = DirState::dirRB;
		uiBulletFrame->Spawn(Vector2(-26.0f, 30.0f));
		uiBulletFrame->img->space = Space::screen;
		uiBulletFrame->img->isVisible = false;

		bulletCount = 7;
		uiBullet.resize(bulletCount);

		int uiBulletIdx = 0;
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

		constexpr float uiWeaponScaleFactor = 2.5f;
		uiWeapon->img = make_shared<ObImage>(L"EnterTheGungeon/Weapon/2/UI_Weapon.png");
		uiWeapon->img->scale = Vector2(44.0f, 15.0f) * uiWeaponScaleFactor;
		uiWeapon->img->pivot = OFFSET_RB;
		uiWeapon->anchor = DirState::dirRB;
		uiWeaponSpawnPos = Vector2(-120.0f, 70.0f);
		uiWeapon->Spawn(uiWeaponSpawnPos);
		uiWeapon->img->space = Space::screen;
		uiWeapon->img->isVisible = false;
	}

	void Weapon2::Update()
	{
		Weapon::Update();
	}

	void Weapon2::Render()
	{
		Weapon::Render();
	}

	void Weapon2::ResizeScreen()
	{
		Weapon::ResizeScreen();
	}
}