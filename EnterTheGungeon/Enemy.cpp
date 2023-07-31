#include "stdafx.h"

namespace Gungeon
{
	Enemy::Enemy()
	{
		Enemy::Init();
	}

	void Enemy::Init()
	{
		InitSelf();
		InitItem();
	}

	void Enemy::InitVar()
	{
		intervalAnim[static_cast<int>(State::idle)] = 0.2f;
		intervalAnim[static_cast<int>(State::walk)] = 0.1f;
		intervalAnim[static_cast<int>(State::die)] = 0.2f;
		intervalHit = 0.2f;
	}

	void Enemy::InitSelf()
	{
		state = State::die;

		col = make_shared<ObCircle>();
		col->isVisible = false;
		col->isFilled = false;
		col->color = Color(1.0f, 1.0f, 1.0f);
		SetPos(DEFAULTSPAWN);

		colTile = make_shared<ObRect>();
		colTile->isVisible = false;
		colTile->scale = Vector2(col->scale.x, col->scale.y / 2.0f);
		colTile->SetParentRT(*col);
		colTile->SetLocalPosY(col->GetWorldPos().y - col->scale.y / 2.0f);
		colTile->isFilled = false;
		colTile->color = Color(1.0f, 1.0f, 1.0f, 1.0f);

		shadow = make_shared<ObImage>(L"EnterTheGungeon/enemy_1/Shadow_1.png");
		shadow->SetParentRT(*col);
	}

	void Enemy::InitItem()
	{
		constexpr float scaleFactor = 0.5f;
		dropItem = make_shared<Item>();
		dropItem->col = make_shared<ObCircle>();
		dropItem->col->isVisible = false;
		dropItem->col->scale = Vector2(40.0f, 40.0f) * scaleFactor;
		dropItem->col->isFilled = false;
		dropItem->SetPos(DEFAULTSPAWN);
		dropItem->idle = make_shared<ObImage>(L"EnterTheGungeon/player_1/UI_Gold.png");
		dropItem->idle->scale = Vector2(40.0f, 40.0f) * scaleFactor;
		dropItem->idle->SetParentRT(*dropItem->col);
		dropItem->idle->isVisible = false;
		dropItem->state = State::die;
	}

	void Enemy::InitIntervalAttack()
	{
	}

	void Enemy::Update()
	{
		Unit::SetTarget(weapon);
		Unit::Update();

		switch (state)
		{
		case State::idle:
			StartAttack();
			Idle();
			break;
		case State::walk:
			StartAttack();
			Walk();
			break;
		case State::attack:
			Attack();
			break;
		case State::die:
			Die();
			break;
		default:
			break;
		}

		if (weapon) 
			weapon->Update();
		for (const auto& elem : bullet) 
			elem->Update();
		dropItem->Update();
	}

	void Enemy::LateUpdate()
	{
	}

	void Enemy::Render()
	{
		for (const auto& elem : bullet) 
			elem->Render();
		Unit::Render();

		if (weapon) 
			weapon->Render();
		dropItem->Render();
	}

	void Enemy::Idle()
	{
		moveDir = targetDir;
		Hitting();
	}

	void Enemy::Walk()
	{
		moveDir = targetDir;
		Hitting();
	}

	void Enemy::Attack()
	{
		Hitting();
		Fire();
	}

	void Enemy::Die()
	{
		Unit::Die();

		timeRealDie += DELTA;
		col->MoveWorldPos(pushedDir * pushedScalar * cos(timeRealDie / 0.63f * DIV2PI) * DELTA);

		if (timeRealDie > 0.63f)
			pushedDir = Vector2(0.0f, 0.0f);
	}

	void Enemy::Fire()
	{
	}

	void Enemy::StartHit(const int damage, const Vector2& dir)
	{
		pushedDir = dir;

		Unit::StartHit(damage);

		hit->ChangeAnim(AnimState::once, 0.1f);

		hit->reverseLR = (pushedDir.x < 0.0f) ? true : false;

		way.clear();

		SOUND.Play("BulletManHurt");
	}

	void Enemy::Hitting()
	{
		if (isHit)
			if (TIMER.GetTick(timeHit, 0.01f))
				isHit = false;

		if (isHitAnim)
		{
			const Color c = Color(RANDOM.Float(0.6f, 1.0f), 0.5f, 0.5f, RANDOM.Float(0.2f, 1.0f));
			idle->color = c;
			walk->color = c;
			hit->color = c;

			idle->isVisible = false;
			walk->isVisible = false;
			hit->isVisible = true;

			timeHitAnim += DELTA;
			col->MoveWorldPos(pushedDir * pushedScalar * cos(timeHitAnim / 0.63f * DIV2PI) * DELTA);

			if (timeHitAnim > 0.63f)
			{
				way.clear();
				constexpr Color color = Color(0.5f, 0.5f, 0.5f, 1.0f);
				idle->color = color;
				walk->color = color;
				hit->color = color;
				die->color = color;

				walk->isVisible = true;
				hit->isVisible = false;

				isHitAnim = false;

				pushedDir = Vector2(0.0f, 0.0f);
			}
		}
		else
		{
			idle->color.w = 1.0f;
			walk->color.w = 1.0f;
			hit->isVisible = false;
		}
	}

	void Enemy::StartDie()
	{
		Unit::StartDie();

		die->ChangeAnim(AnimState::once, intervalAnim[static_cast<int>(State::die)]);

		way.clear();

		if (weapon) 
			weapon->idle->isVisible = false;

		die->reverseLR = (pushedDir.x < 0.0f) ? true : false;

		dropItem->Spawn(Pos());
		dropItem->idle->isVisible = true;
		dropItem->state = State::idle;

		SOUND.Play("BulletManDeath");
	}

	void Enemy::Spawn(const Vector2 wpos)
	{
		Unit::Spawn(wpos);

		InitVar();

		if (weapon) 
			weapon->idle->isVisible = true;

		die->reverseLR = (pushedDir.x < 0.0f) ? true : false;

		dropItem->idle->isVisible = false;
		dropItem->state = State::die;
	}

	void Enemy::AttackAnimStart() const
	{
		idle->isVisible = false;
		walk->isVisible = false;
		attack->isVisible = true;
		attack->ChangeAnim(AnimState::loop, 0.1f);
	}

	void Enemy::AttackAnimEnd() const
	{
		attack->isVisible = false;
		attackEnd->isVisible = true;
		attackEnd->ChangeAnim(AnimState::once, 0.1f);
	}

	void Enemy::AttackToWalk() const
	{
		attackEnd->isVisible = false;
		walk->isVisible = true;
	}

	void Enemy::ColToggle()
	{
		Character::ColToggle();
		if (weapon) 
			weapon->col->isVisible ^= 1;
		if (weapon) 
			weapon->firePos->isVisible ^= 1;
		for (const auto& bulletElem : bullet)
			bulletElem->col->isVisible ^= 1;
	}
}