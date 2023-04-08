#include "stdafx.h"

namespace Gungeon
{
	Enemy1::Enemy1()
	{
		Enemy1::Init();
	}

	void Enemy1::Init()
	{
		InitVar();
		InitSelf();
		InitWeapon();
		InitBullet();
		InitIntervalAttack();
	}

	void Enemy1::InitVar()
	{
		curHp = maxHp = 3;
		scalar = 80.0f;
	}

	void Enemy1::InitSelf()
	{
		int idx = 0;

		float scaleFactor = 3.0f;
		col->scale = Vector2(16.0f, 16.0f) * scaleFactor;

		idle = make_shared<ObImage>(L"EnterTheGungeon/enemy_1/Idle.png");
		idle->isVisible = false;
		idle->maxFrame = Vec2i(2, 8);
		idle->scale = Vector2(28.0f / 2.0f, 192.0f / 8.0f) * scaleFactor;
		idle->ChangeAnim(AnimState::loop, 0.2f);
		idle->SetParentRT(*col);

		walk = make_shared<ObImage>(L"EnterTheGungeon/enemy_1/Walk.png");
		walk->isVisible = false;
		walk->maxFrame.x = 6;
		walk->maxFrame = Vec2i(6, 8);
		walk->scale = Vector2(96.0f / 6.0f, 192.0f / 8.0f) * scaleFactor;
		walk->scale.y = 24.0f * scaleFactor;
		walk->ChangeAnim(AnimState::loop, 0.1f);
		walk->SetParentRT(*col);

		hit = make_shared<ObImage>(L"EnterTheGungeon/enemy_1/Hit.png");
		hit->isVisible = false;
		hit->maxFrame.x = 1;
		hit->scale = Vector2(16.0f, 24.0f) * scaleFactor;
		hit->ChangeAnim(AnimState::once, 0.2f);
		hit->SetParentRT(*col);

		die = make_shared<ObImage>(L"EnterTheGungeon/enemy_1/Die.png");
		die->isVisible = false;
		die->maxFrame.x = 5;
		die->scale = Vector2(110.0f / 5.0f, 22.0f) * scaleFactor;
		die->SetParentRT(*col);

		shadow->scale = Vector2(12.0f, 4.0f) * scaleFactor;
		shadow->SetLocalPosY(-35.0f);
	}

	void Enemy1::InitWeapon()
	{
		weapon = make_shared<Weapon1>();
		weapon->col->SetParentRT(*col);
		weapon->col->SetLocalPos(Vector2(10.0f, -15.0f));
		weapon->idle->SetParentRT(*weapon->col);
		weapon->firePos->SetLocalPos(Vector2(weapon->col->scale.x / 2.0f, 0.0f));
		weapon->fireEffect->idle->SetParentRT(*weapon->firePos);
		weapon->imgReloading->SetParentRT(*weapon->col);
		weapon->Equip();
	}

	void Enemy1::InitBullet()
	{
		bullet.resize(bulletMax);
		for (auto& elem : bullet)
			elem = make_shared<EnemyBullet>();
	}

	void Enemy1::InitIntervalAttack()
	{
		intervalAttackStart = RANDOM.Float(1.5f, 2.0f);
	}

	void Enemy1::Update()
	{
		Enemy::Update();
	}

	void Enemy1::LateUpdate()
	{
		Enemy::LateUpdate();
	}

	void Enemy1::Render()
	{
		Enemy::Render();
	}

	void Enemy1::Fire()
	{
		weapon->fireEffect->Spawn(weapon->firePos->GetWorldPos());

		bullet[curBulletIdx++]->Spawn(weapon->firePos->GetWorldPos(), moveDir);

		if (curBulletIdx >= bulletMax) 
			curBulletIdx = 0;

		InitIntervalAttack();

		SOUND.Stop("Pistol");
		SOUND.Play("Pistol");

		state = State::idle;
	}
}