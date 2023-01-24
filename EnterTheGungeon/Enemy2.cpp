#include "stdafx.h"

namespace Gungeon
{
	Enemy2::Enemy2()
	{
		Enemy2::Init();
	}

	void Enemy2::Init()
	{
		InitVar();
		InitSelf();
		InitWeapon();
		InitBullet();
		InitIntervalAttack();
	}

	void Enemy2::InitVar()
	{
		curHp = maxHp = 3;
		scalar = 80.0f;
	}

	void Enemy2::InitSelf()
	{
		int idx = 0;

		float scaleFactor = 3.0f;
		col->scale = Vector2(16.0f, 16.0f) * scaleFactor;

		idle = make_shared<ObImage>(L"EnterTheGungeon/enemy_2/Idle.png");
		idle->isVisible = false;
		idle->maxFrame = Int2(2, 8);
		idle->scale = Vector2(24.0f / 2.0f, 240.0f / 8.0f) * scaleFactor;
		idle->ChangeAnim(AnimState::loop, 0.2f);
		idle->SetParentRT(*col);

		walk = make_shared<ObImage>(L"EnterTheGungeon/enemy_2/Walk.png");
		walk->isVisible = false;
		walk->maxFrame = Int2(6, 8);
		walk->scale = Vector2(108.0f / 6.0f, 240.0f / 8.0f) * scaleFactor;
		walk->ChangeAnim(AnimState::loop, 0.1f);
		walk->SetParentRT(*col);

		hit = make_shared<ObImage>(L"EnterTheGungeon/enemy_2/Hit.png");
		hit->isVisible = false;
		hit->maxFrame.x = 1;
		hit->scale = Vector2(17.0f, 24.0f) * scaleFactor;
		hit->ChangeAnim(AnimState::once, 0.2f);
		hit->SetParentRT(*col);

		die = make_shared<ObImage>(L"EnterTheGungeon/enemy_2/Die.png");
		die->isVisible = false;
		die->maxFrame.x = 4;
		die->scale = Vector2(84.0f / 4.0f, 24.0f) * scaleFactor;
		die->SetParentRT(*col);

		shadow->scale = Vector2(12.0f, 4.0f) * scaleFactor;
		shadow->SetLocalPosY(-45.0f);
	}

	void Enemy2::InitWeapon()
	{
		weapon = make_shared<Weapon2>();
		weapon->col->SetParentRT(*col);
		weapon->col->SetLocalPos(Vector2(10.0f, -15.0f));
		weapon->idle->SetParentRT(*weapon->col);
		weapon->firePos->SetLocalPos(Vector2(weapon->col->scale.x / 2.0f, 0.0f));
		weapon->fireEffect->idle->SetParentRT(*weapon->firePos);
		weapon->imgReloading->SetParentRT(*weapon->col);
		weapon->Equip();
	}

	void Enemy2::InitBullet()
	{
		bullet.resize(bulletMax);
		for (auto& elem : bullet)
		{
			elem = make_shared<EnemyBullet>();
		}
	}

	void Enemy2::InitIntervalAttack()
	{
		intervalAttackStart = RANDOM.Float(2.0f, 3.0f);
	}

	void Enemy2::Update()
	{
		Enemy::Update();
	}

	void Enemy2::LateUpdate()
	{
		Enemy::LateUpdate();
	}

	void Enemy2::Render()
	{
		Enemy::Render();
	}

	void Enemy2::Fire()
	{
		weapon->fireEffect->Spawn(weapon->firePos->GetWorldPos());

		int t = 5;
		while (t--)
		{
			bullet[curBulletIdx]->scalar = RANDOM.Float(600.0f, 800.0f);
			Vector2 fireDir = Vector2(RANDOM.Float(moveDir.x - 0.2f, moveDir.x + 0.2f),
				RANDOM.Float(moveDir.y - 0.2f, moveDir.y + 0.2f));
			bullet[curBulletIdx]->Spawn(weapon->firePos->GetWorldPos(), fireDir);
			curBulletIdx++;
		}

		if (curBulletIdx >= bulletMax) curBulletIdx = 0;

		InitIntervalAttack();

		SOUND.Stop("Shotgun");
		SOUND.Play("Shotgun");

		state = State::idle;
	}
}