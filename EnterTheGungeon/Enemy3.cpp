#include "stdafx.h"

namespace Gungeon
{
	Enemy3::Enemy3()
	{
		Enemy3::Init();
	}

	void Enemy3::Init()
	{
		InitVar();
		InitSelf();
		InitWeapon();
		InitBullet();
		InitIntervalAttack();
	}

	void Enemy3::InitVar()
	{
		curHp = maxHp = 3;
		scalar = 100.0f;
	}

	void Enemy3::InitSelf()
	{
		int idx = 0;

		constexpr float scaleFactor = 3.0f;
		col->scale = Vector2(16.0f, 16.0f) * scaleFactor;

		idle = make_shared<ObImage>(L"EnterTheGungeon/enemy_3/Idle.png");
		idle->isVisible = false;
		idle->maxFrame = Vec2i(4, 8);
		idle->scale = Vector2(52.0f / 4.0f, 112.0f / 8.0f) * scaleFactor;
		idle->ChangeAnim(AnimState::loop, 0.2f);
		idle->SetParentRT(*col);

		walk = make_shared<ObImage>(L"EnterTheGungeon/enemy_3/Walk.png");
		walk->isVisible = false;
		walk->maxFrame = Vec2i(4, 8);
		walk->scale = Vector2(52.0f / 4.0f, 112.0f / 8.0f) * scaleFactor;
		walk->ChangeAnim(AnimState::loop, 0.1f);
		walk->SetParentRT(*col);

		hit = make_shared<ObImage>(L"EnterTheGungeon/enemy_3/Hit.png");
		hit->isVisible = false;
		hit->maxFrame.x = 1;
		hit->scale = Vector2(13.0f, 14.0f) * scaleFactor;
		hit->ChangeAnim(AnimState::once, 0.2f);
		hit->SetParentRT(*col);

		die = make_shared<ObImage>(L"EnterTheGungeon/enemy_3/Die.png");
		die->isVisible = false;
		die->maxFrame.x = 2;
		die->scale = Vector2(30.0f / 2.0f, 16.0f) * scaleFactor;
		die->SetParentRT(*col);

		attack = make_shared<ObImage>(L"EnterTheGungeon/enemy_3/Attack.png");
		attack->isVisible = false;
		attack->maxFrame.x = 13;
		attack->scale = Vector2(247.0f / 13.0f, 19.0f) * scaleFactor;
		attack->SetParentRT(*col);

		attackEnd = make_shared<ObImage>(L"EnterTheGungeon/enemy_3/Attack_End.png");
		attackEnd->isVisible = false;
		attackEnd->maxFrame.x = 9;
		attackEnd->scale = Vector2(153.0f / 9.0f, 23.0f) * scaleFactor;
		attackEnd->SetParentRT(*col);

		shadow->scale = Vector2(12.0f, 4.0f) * scaleFactor;
		shadow->SetLocalPosY(-45.0f);
	}

	void Enemy3::InitWeapon()
	{
		weapon = nullptr;
	}

	void Enemy3::InitBullet()
	{
		bullet.resize(bulletMax);
		for (auto& elem : bullet)
		{
			elem = make_shared<EnemyBullet>();
			elem->idle->SetParentRT(*elem->col);
			elem->idle->color = Color(0.8f, 0.6f, 0.4f);
		}
	}

	void Enemy3::InitIntervalAttack()
	{
		intervalAttackStart = RANDOM.Float(1.5f, 3.0f);
	}

	void Enemy3::Update()
	{
		Enemy::Update();
		attack->Update();
		attackEnd->Update();
	}

	void Enemy3::LateUpdate()
	{
		Enemy::LateUpdate();
	}

	void Enemy3::Render()
	{
		Enemy::Render();
		attack->Render();
		attackEnd->Render();
	}

	void Enemy3::Fire()
	{
		switch (fireState)
		{
		case Gungeon::Enemy3FireState::none:

			AttackAnimStart();

			spawnPos = Pos();
			for (auto& elem : bullet)
			{
				elem->scalar = 0.0f;
				elem->moveDir = Vector2(0.0f, 0.0f);
			}

			idle->isVisible = false;
			walk->isVisible = false;
			fireState = Enemy3FireState::z;

			break;

		case Gungeon::Enemy3FireState::z:

			if (TIMER.GetTick(timeBulletZ, 0.05f))
			{
				auto plusXY = [&]()
				{
					if (++curBulletX >= 5)
					{
						curBulletX = 0;
						if (++curBulletY >= 5)
						{
							curBulletY = 0;
							AttackAnimEnd();
							fireState = Enemy3FireState::circle;
						}
					}
					curBulletIdx++;
				};
				while (0 == stringBullet.Z[curBulletY][curBulletX])
				{
					plusXY();
				}

				bullet[curBulletIdx]->Spawn(Vector2(spawnPos.x - 50.0f + curBulletX * 20.0f,
					spawnPos.y + 50.0f - curBulletY * 20.0f));

				SOUND.Stop("BookCharge");
				SOUND.Play("BookCharge");

				plusXY();
			}

			break;

		case Gungeon::Enemy3FireState::circle:

			if (TIMER.GetTick(timeBulletCircle, 0.05f))
			{
				bullet[curBulletIdx++]->Spawn(Vector2(
					spawnPos.x - 10.0f + 80.0f * cos((curBulletIdx - 25) * 20.0f * ToRadian),
					spawnPos.y + 10.0f + 80.0f * sin((curBulletIdx - 25) * 20.0f * ToRadian)));

				if (curBulletIdx >= bulletMax)
				{
					curBulletIdx = 0;
					fireState = Enemy3FireState::target;
				}

				SOUND.Stop("BookCharge");
				SOUND.Play("BookCharge");
			}

			break;

		case Gungeon::Enemy3FireState::target:

			for (const auto& elem : bullet)
			{
				elem->scalar = 700.0f;
				elem->moveDir = targetDir;
			}

			SOUND.Stop("BookCharge");
			SOUND.Play("BookAttack");

			fireState = Enemy3FireState::toWalk;

			break;

		case Gungeon::Enemy3FireState::toWalk:

			for (const auto& elem : bullet)
				elem->moveFactor = cos(timeAttackToWalk / 1.5f * DIV2PI);

			if (TIMER.GetTick(timeAttackToWalk, 1.5f))
			{
				AttackToWalk();
				InitIntervalAttack();
				state = State::walk;
				fireState = Enemy3FireState::none;
			}

			break;
		}
	}

	void Enemy3::StartDie()
	{
		Enemy::StartDie();

		SOUND.Play("BookDrop");
	}
}