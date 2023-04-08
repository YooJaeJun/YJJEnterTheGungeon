#include "stdafx.h"

namespace Gungeon
{
	Unit::Unit()
	{
		Init();
	}

	void Unit::Init()
	{
		intervalAnim[(int)State::idle] = 0.0f;
		intervalAnim[(int)State::walk] = 0.0f;
		intervalAnim[(int)State::roll] = 0.0f;
		intervalAnim[(int)State::attack] = 0.0f;
		intervalAnim[(int)State::die] = 2.0f;
		intervalAnim[(int)State::cinematic] = 0.0f;
	}

	void Unit::Update()
	{
		SetLastPos();
		SetLastDir();

		switch (state)
		{
		case Gungeon::State::idle:
			if (false == (moveDir.x == 0.0f && moveDir.y == 0.0f))
				StartWalk();
			break;
		case Gungeon::State::walk:
			if (moveDir.x == 0.0f && moveDir.y == 0.0f)
				StartIdle();
			break;
		}

		Character::Update();

		if (respawn) 
			respawn->Update();
		idle->Update();
		walk->Update();
		if (hit) 
			hit->Update();
		if (fall) 
			fall->Update();
		if (die) 
			die->Update();
		if (shadow) 
			shadow->Update();
	}

	void Unit::LateUpdate()
	{
	}

	void Unit::Render()
	{
		if (respawn) 
			respawn->Render();
		idle->Render();
		walk->Render();
		if (hit)  
			hit->Render();
		if (fall) 
			fall->Render();
		if (die)  
			die->Render();
		Character::Render();
	}

	void Unit::ResizeScreen()
	{
	}

	void Unit::SetTarget(shared_ptr<Weapon>& weapon)
	{
		targetDir = targetPos - Pos();
		targetDir.Normalize();
		SetDirState(targetDir, curTargetDirState);

		idle->frame.y = curTargetDirState;
		walk->frame.y = curTargetDirState;

		if (weapon)
		{
			weapon->col->rotation = Utility::DirToRadian(targetPos - weapon->Pos());

			if (targetDir.x >= 0.0f)
			{
				if (targetDirBefore.x < 0.0f)
					weapon->EquipRight();
			}
			else
			{
				if (targetDirBefore.x >= 0.0f)
					weapon->EquipLeft();
			}
		}
	}

	void Unit::Idle()
	{
	}

	void Unit::Walk()
	{
	}

	void Unit::Attack()
	{
	}

	void Unit::StartHit(const int damage)
	{
		if (state != State::die && 
			false == isHit)
		{
			curHp -= damage;

			if (damage > 0)
			{
				isHit = true;
				isHitAnim = true;

				if (hit)
				{
					idle->isVisible = false;
					walk->isVisible = false;
					hit->isVisible = true;
					hit->ChangeAnim(AnimState::loop, intervalHit);
				}

				if (curHp <= 0)
				{
					curHp = 0;
					StartDie();
				}
			}
		}
	}

	void Unit::StartDie()
	{
		state = State::die;

		isHit = false;

		idle->isVisible = false;
		walk->isVisible = false;
		if (attack) 
			attack->isVisible = false;
		if (attackEnd) 
			attackEnd->isVisible = false;
		if (hit) 
			hit->isVisible = false;
		shadow->isVisible = false;
		die->isVisible = true;

		for (auto& elem : bullet)
		{
			elem->idle->isVisible = false;
			elem->SetPos(DEFAULTSPAWN);
		}
	}

	void Unit::SetLastDir()
	{
		curMoveDirStateBefore = curMoveDirState;
		curTargetDirStateBefore = curTargetDirState;
		targetDirBefore = targetDir;
	}

	void Unit::Die()
	{
		if (TIMER.GetTick(timeDieAnim, 0.1f))
			die->color = Color(0.4f, 0.4f, 0.4f, 0.4f);
	}

	void Unit::StartWalk()
	{
		state = State::walk;
		idle->isVisible = false;
		walk->isVisible = true;
		walk->ChangeAnim(AnimState::loop, intervalAnim[(int)State::walk]);
	}

	void Unit::StartIdle()
	{
		state = State::idle;
		walk->isVisible = false;
		idle->isVisible = true;
		idle->ChangeAnim(AnimState::loop, intervalAnim[(int)State::idle]);
	}

	void Unit::StartAttack()
	{
		if (TIMER.GetTick(timeAttackStart, intervalAttackStart))
			state = State::attack;
	}

	// moveDir: 이동방향에 따라 다른 방향 애니메이션(플레이어는 roll) 설정
	// targetDir: 타겟 좌표(플레이어는 마우스)에 따라 다른 방향 애니메이션(공통 Idle, Walk) 설정
	void Unit::SetDirState(const Vector2 dir, DirState& dirState)
	{
		float rotation = Utility::DirToRadian(dir);

		if (rotation >= 30.0f * ToRadian && rotation < 60.0f * ToRadian)
			dirState = dirRT;
		else if (rotation >= 120.0f * ToRadian && rotation < 150.0f * ToRadian)
			dirState = dirLT;
		else if (rotation >= -150.0f * ToRadian && rotation < -120.0f * ToRadian)
			dirState = dirLB;
		else if (rotation >= -60.0f * ToRadian && rotation < -30.0f * ToRadian)
			dirState = dirRB;

		else if (rotation >= -30.0f * ToRadian && rotation < 30.0f * ToRadian)
			dirState = dirR;
		else if ((rotation > -181.0f * ToRadian && rotation <= -150.0f * ToRadian) ||
			(rotation >= 150.0f * ToRadian && rotation < 181.0f * ToRadian))
			dirState = dirL;
		else if (rotation >= 60.0f * ToRadian && rotation < 120.0f * ToRadian)
			dirState = dirT;
		else if (rotation >= -120.0f * ToRadian && rotation < -60.0f * ToRadian)
			dirState = dirB;
	}

	void Unit::FindPath(std::shared_ptr<ObTileMap> map)
	{
		if (TIMER.GetTick(timeFindPath, 1.0f))
		{
			Vec2i sour, dest;
			bool isFind = map->WorldPosToTileIdx(Pos(), sour);
			isFind &= map->WorldPosToTileIdx(targetPos, dest);

			if (isFind)
			{
				if (ASTAR.FindPath(map, sour, dest, way))
				{
					g = 0.0f;
					start = Pos();
					way.pop_back();
					map->Update();
					end = way.back().Pos;
				}
			}
		}

		if (false == way.empty())
		{
			SetPos(Vector2::Lerp(start, end, g));
			moveDir = end - start;
			moveDir.Normalize();

			g += DELTA * scalar / 100.0f;

			if (g > 1.0f)
			{
				g = 0.0f;
				start = end;
				way.pop_back();
				if (false == way.empty())
					end = way.back().Pos;
			}
		}
	}
	
	void Unit::Spawn(const Vector2 wpos)
	{
		Character::Spawn(wpos);

		die->isVisible = false;
		idle->isVisible = true;
		shadow->isVisible = true;

		Color c = Color(0.5f, 0.5f, 0.5f, 1.0f);
		idle->color = c;
		walk->color = c;
		if (hit) 
			hit->color = c;
		if (die) 
			die->color = c;
	}

	void Unit::Stop()
	{
		moveDir = Vector2(0.0f, 0.0f);
	}
}