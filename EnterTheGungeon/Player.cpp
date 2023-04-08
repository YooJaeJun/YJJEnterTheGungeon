#include "stdafx.h"

namespace Gungeon
{
	Player::Player()
	{
		Init();
	}

	void Player::Init()
	{
		InitVar();
		InitCol();
		InitAnim();
		InitWeapon();
		InitBullet();
		InitEffect();
		InitUI();
	}

	void Player::InitVar()
	{
		scalar = 300.0f;
		curHp = maxHp = 6;
		for (auto& elem : canFireOnce) elem = true;

		intervalAnim[(int)State::idle] = 0.2f;
		intervalAnim[(int)State::walk] = 0.1f;
		intervalAnim[(int)State::roll] = 0.07f;
		intervalAnim[(int)State::attack] = 0.0f;
		intervalAnim[(int)State::die] = 0.2f;
		intervalAnim[(int)State::fall] = 0.3f;
		intervalAnim[(int)State::respawn] = 0.3f;
		intervalAnim[(int)State::cinematic] = 0.0f;
	}

	void Player::InitCol()
	{
		float scaleFactor = 3.0f;
		col = make_shared<ObCircle>();
		col->isVisible = false;
		col->isFilled = false;
		col->scale = Vector2(12.0f, 12.0f) * scaleFactor;
		col->color = Color(1.0f, 1.0f, 1.0f);

		colTile = make_shared<ObRect>();
		colTile->isVisible = false;
		colTile->scale = Vector2(col->scale.x * 0.8f, col->scale.y / 2.0f + 15.0f);
		colTile->SetParentRT(*col);
		colTile->SetLocalPosY(-20.0f);
		colTile->isFilled = false;
		colTile->color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void Player::InitAnim()
	{
		int idx = 0;
		float scaleFactor = 3.0f;

		idle = make_shared<ObImage>(L"EnterTheGungeon/player_1/Idle.png");
		idle->maxFrame = Vec2i(4, 8);
		idle->scale = Vector2(72.0 / 4.0f, 160.0f / 8.0f) * scaleFactor;
		idle->ChangeAnim(AnimState::loop, intervalAnim[(int)State::idle]);
		idle->SetParentRT(*col);

		walk = make_shared<ObImage>(L"EnterTheGungeon/player_1/Walk.png");
		walk->isVisible = false;
		walk->maxFrame = Vec2i(6, 8);
		walk->scale = Vector2(102.0f / 6.0f, 192.0f / 8.0f) * scaleFactor;
		walk->ChangeAnim(AnimState::loop, intervalAnim[(int)State::walk]);
		walk->SetParentRT(*col);

		roll = make_shared<ObImage>(L"EnterTheGungeon/player_1/Roll.png");
		roll->isVisible = false;
		roll->maxFrame = Vec2i(9, 8);
		roll->scale = Vector2(180.0f / 9.0f, 192.0f / 8.0f) * scaleFactor;
		roll->SetParentRT(*col);

		die = make_shared<ObImage>(L"EnterTheGungeon/player_1/Die.png");
		die->isVisible = false;
		die->maxFrame.x = 8;
		die->scale = Vector2(160.0f / 8.0f, 24.0f) * scaleFactor;
		die->SetParentRT(*col);

		fall = make_shared<ObImage>(L"EnterTheGungeon/player_1/Fall.png");
		fall->isVisible = false;
		fall->maxFrame.x = 6;
		fall->scale = Vector2(96.0f / 6.0f, 18.0f) * scaleFactor;
		fall->SetParentRT(*col);

		respawn = make_shared<ObImage>(L"EnterTheGungeon/player_1/Spawn.png");
		respawn->isVisible = false;
		respawn->maxFrame.x = 6;
		respawn->scale = Vector2(96.0f / 6.0f, 26.0f) * scaleFactor;
		respawn->SetParentRT(*col);
		respawn->SetLocalPosY(6.0f);
	}

	void Player::InitWeapon()
	{
		curWeaponIdx = 0;
		weapons[curWeaponIdx] = make_shared<Weapon1>();
		weapons[curWeaponIdx]->col->SetParentRT(*col);
		weapons[curWeaponIdx]->col->SetLocalPos(Vector2(10.0f, -15.0f));
		weapons[curWeaponIdx]->idle->isVisible = true;
		weapons[curWeaponIdx]->Equip();

		for (auto& elem : weapons[curWeaponIdx]->uiBullet)
			elem->img->isVisible = true;

		weapons[curWeaponIdx]->uiBulletFrame->img->isVisible = true;
		weapons[curWeaponIdx]->uiWeapon->img->isVisible = true;
	}

	void Player::InitBullet()
	{
		curBulletIdx = weapons[curWeaponIdx]->bulletCount - 1;
		bullet.resize(weapons[curWeaponIdx]->bulletCount);
		for (auto& elem : bullet)
			elem = make_shared<PlayerBullet>();
	}

	void Player::InitEffect()
	{
		float scaleFactor = 2.0f;
		shadow = make_shared<ObImage>(L"EnterTheGungeon/player_1/Shadow.png");
		shadow->scale.x = 16.0f * scaleFactor;
		shadow->scale.y = 5.0f * scaleFactor;
		shadow->SetParentRT(*col);
		shadow->SetWorldPosY(-30.0f);

		float dustScaleFactor = 1.0f;
		for (auto& elem : dust)
		{
			elem = make_shared<Effect>();
			elem->idle = make_shared<ObImage>(L"EnterTheGungeon/player_1/Dust.png");
			elem->idle->maxFrame.x = 7;
			elem->idle->scale = Vector2(189.0f / 7.0f, 27.0f) * dustScaleFactor;
			elem->idle->isVisible = false;
			elem->idle->ChangeAnim(AnimState::loop, 0.2f);
			elem->intervalDie = 0.8f;
		}
	}

	void Player::InitUI()
	{
		int idx = 0;

		uiReload = make_shared<UI>();
		uiReload->img = make_shared<ObImage>(L"EnterTheGungeon/player_1/UI_Reload.png");
		uiReload->img->isVisible = false;
		uiReload->img->scale = Vector2(144.0f, 20.0f);
		uiReload->img->SetParentT(*col);
		uiReload->img->SetLocalPosX(0.0f);
		uiReload->img->SetLocalPosY(60.0f);

		uiReloadBar = make_shared<UI>();
		uiReloadBar->img = make_shared<ObImage>(L"EnterTheGungeon/player_1/UI_ReloadBar.png");
		uiReloadBar->img->isVisible = false;
		uiReloadBar->img->scale = Vector2(4.0f, 20.0f);
		uiReloadBar->img->SetParentT(*col);
		uiReloadBar->img->SetLocalPosX(-60.0f);
		uiReloadBar->img->SetLocalPosY(60.0f);

		uiWeaponFrame = make_shared<UI>();
		uiWeaponFrame->img = make_shared<ObImage>(L"EnterTheGungeon/Weapon/UI_WeaponFrame.png");
		uiWeaponFrame->img->scale = Vector2(188.0f, 116.0f);
		uiWeaponFrame->img->pivot = OFFSET_RB;
		uiWeaponFrame->anchor = DirState::dirRB;
		uiWeaponFrame->Spawn(Vector2(-70.0f, 30.0f));
		uiWeaponFrame->img->space = Space::screen;
		uiWeaponFrame->img->isVisible = true;

		for (auto& elem : weapons[curWeaponIdx]->uiBullet)
			elem->img->isVisible = true;

		uiHeartNone.resize(maxHp / 2);
		uiHeartHalf.resize(maxHp / 2);
		uiHeartFull.resize(maxHp / 2);

		float heartScaleFactor = 1.0f;
		idx = 0;
		for (auto& elem : uiHeartNone)
		{
			elem = make_shared<UI>();
			elem->img = make_shared<ObImage>(L"EnterTheGungeon/player_1/HeartNone.png");
			elem->anchor = DirState::dirLT;
			elem->Spawn(Vector2(10.0f + idx * 60.0f, -40.0f));
			elem->img->pivot = OFFSET_L;
			elem->img->scale.x = 52.0f * heartScaleFactor;
			elem->img->scale.y = 44.0f * heartScaleFactor;
			elem->img->space = Space::screen;
			idx++;
		}
		idx = 0;
		for (auto& elem : uiHeartHalf)
		{
			elem = make_shared<UI>();
			elem->img = make_shared<ObImage>(L"EnterTheGungeon/player_1/HeartHalf.png");
			elem->anchor = DirState::dirLT;
			elem->Spawn(Vector2(10.0f + idx * 60.0f, -40.0f));
			elem->img->pivot = OFFSET_L;
			elem->img->scale.x = 52.0f * heartScaleFactor;
			elem->img->scale.y = 44.0f * heartScaleFactor;
			elem->img->space = Space::screen;
			idx++;
		}
		idx = 0;
		for (auto& elem : uiHeartFull)
		{
			elem = make_shared<UI>();
			elem->img = make_shared<ObImage>(L"EnterTheGungeon/player_1/HeartFull.png");
			elem->anchor = DirState::dirLT;
			elem->Spawn(Vector2(10.0f + idx * 60.0f, -40.0f));
			elem->img->pivot = OFFSET_L;
			elem->img->scale.x = 52.0f * heartScaleFactor;
			elem->img->scale.y = 44.0f * heartScaleFactor;
			elem->img->space = Space::screen;
			idx++;
		}

		idx = 0;
		uiBlank.resize(2);
		for (auto& elem : uiBlank)
		{
			elem = make_shared<UI>();
			elem->img = make_shared<ObImage>(L"EnterTheGungeon/player_1/UI_Blank.png");
			elem->anchor = DirState::dirLT;
			elem->Spawn(Vector2(10.0f + idx * 50.0f, -100.0f));
			elem->img->pivot = OFFSET_L;
			elem->img->scale.x = 40.0f;
			elem->img->scale.y = 40.0f;
			elem->img->space = Space::screen;
			idx++;
		}

		uiKey = make_shared<UI>();
		uiKey->img = make_shared<ObImage>(L"EnterTheGungeon/player_1/UI_Key.png");
		uiKey->anchor = DirState::dirLT;
		uiKey->Spawn(Vector2(10.0f, -160.0f));
		uiKey->img->pivot = OFFSET_L;
		uiKey->img->scale.x = 56.0f;
		uiKey->img->scale.y = 48.0f;
		uiKey->img->space = Space::screen;

		uiGold = make_shared<UI>();
		uiGold->img = make_shared<ObImage>(L"EnterTheGungeon/player_1/UI_Gold.png");
		uiGold->anchor = DirState::dirLT;
		uiGold->Spawn(Vector2(120.0f, -160.0f));
		uiGold->img->pivot = OFFSET_L;
		uiGold->img->scale.x = 40.0f;
		uiGold->img->scale.y = 40.0f;
		uiGold->img->space = Space::screen;
	}

	void Player::Update()
	{
		Unit::Update();

		LIGHT.SetLightPos(col->GetWorldPivot());

		SetTargetAndCamera();

		switch (state)
		{
		case State::idle:
			Idle();
			break;
		case State::walk:
			Walk();
			break;
		case State::roll:
			Roll();
			break;
		case State::fall:
			Fall();
			break;
		case State::respawn:
			Respawn();
			break;
		case State::die:
			Die();
			break;
		}

		SetWeaponFrameToOrigin();

		if (INPUT.KeyDown('P'))
			godModeByForce ^= 1;
		if (godModeByForce)
			godMode = true;

		for (auto& elem : dust) 
			elem->Update();
		roll->Update();
		respawn->Update();
		fall->Update();
		respawn->Update();

		weapons[curWeaponIdx]->Update();
		for (auto& elem : bullet) 
			elem->Update();
		uiReload->Update();
		uiReloadBar->Update();
		if (uiWeaponFrame) 
			uiWeaponFrame->Update();
		for (auto& elem : uiHeartNone) 
			elem->Update();
		for (auto& elem : uiHeartHalf) 
			elem->Update();
		for (auto& elem : uiHeartFull) 
			elem->Update();
		for (auto& elem : uiBlank) 
			elem->Update();
		uiKey->Update();
		uiGold->Update();
	}

	void Player::LateUpdate()
	{
	}

	void Player::Render()
	{
		for (auto& elem : dust) 
			elem->Render();

		Unit::Render();

		roll->Render();
		respawn->Render();
		fall->Render();
		respawn->Render();
		for (auto& elem : bullet) 
			elem->Render();

		weapons[curWeaponIdx]->Render();

		if (isUIRendering)
		{
			uiReload->Render();
			uiReloadBar->Render();
			if (uiWeaponFrame) 
				uiWeaponFrame->Render();
			for (auto& elem : uiHeartNone) 
				elem->Render();
			for (auto& elem : uiHeartHalf) 
				elem->Render();
			for (auto& elem : uiHeartFull) 
				elem->Render();
			for (auto& elem : uiBlank) 
				elem->Render();
			uiKey->Render();
			uiGold->Render();
		}

		if (state != State::cinematic)
		{
			DWRITE.RenderText(to_wstring(money),
				RECT{ 170, 140, (long)app.GetWidth(), (long)app.GetHeight() },
				40.0f,
				L"Alagard");

			DWRITE.RenderText(to_wstring(key),
				RECT{ 80, 140, (long)app.GetWidth(), (long)app.GetHeight() },
				40.0f,
				L"Alagard");

			if (weapons[curWeaponIdx]->remainBulletCount == INT_MAX)
				weapons[curWeaponIdx]->uiBulletCountInfinity->img->isVisible = true;
			else
			{
				DWRITE.RenderText(to_wstring(weapons[curWeaponIdx]->remainBulletCount),
					RECT{ (long)app.GetWidth() - 140, (long)app.GetHeight() - 190,
					(long)app.GetWidth(), (long)app.GetHeight() },
					45.0f,
					L"Alagard");
			}
		}

		if (flagInteractionUI)
		{
			DWRITE.RenderText(L"E키 입력",
				RECT{ (long)app.GetHalfWidth() + 30, (long)app.GetHalfHeight() + 10,
				(long)app.GetWidth(), (long)app.GetHeight() },
				30.0f,
				L"PF스타더스트");
		}
	}

	void Player::ResizeScreen()
	{
		weapons[curWeaponIdx]->ResizeScreen();
		uiWeaponFrame->Spawn(Vector2(-70.0f, 30.0f));

		int idx = 0;
		for (auto& elem : uiHeartNone)
		{
			elem->Spawn(Vector2(10.0f + idx * 60.0f, -40.0f));
			idx++;
		}
		idx = 0;
		for (auto& elem : uiHeartHalf)
		{
			elem->Spawn(Vector2(10.0f + idx * 60.0f, -40.0f));
			idx++;
		}
		idx = 0;
		for (auto& elem : uiHeartFull)
		{
			elem->Spawn(Vector2(10.0f + idx * 60.0f, -40.0f));
			idx++;
		}

		idx = 0;
		for (auto& elem : uiBlank)
		{
			elem->Spawn(Vector2(10.0f + idx * 50.0f, -100.0f));
			idx++;
		}
		uiKey->Spawn(Vector2(10.0f, -160.0f));
		uiGold->Spawn(Vector2(120.0f, -160.0f));
	}

	void Player::SetTargetAndCamera()
	{
		if (state != State::cinematic)
		{
			targetPos = INPUT.GetWorldMousePos();

			Vector2 camTargetPos = (targetPos + Pos()) / 2.0f;
			Vector2 velocity = camTargetPos - CAM.position;
			CAM.position += velocity * 4.0f * DELTA;

			CAM.position.x = Utility::Saturate(CAM.position.x,
				idle->GetWorldPos().x - 250.0f,
				idle->GetWorldPos().x + 250.0f);
			CAM.position.y = Utility::Saturate(CAM.position.y,
				idle->GetWorldPos().y - 250.0f,
				idle->GetWorldPos().y + 250.0f);
		}
	}

	void Player::Idle()
	{
		Unit::SetTarget(weapons[curWeaponIdx]);
		Move();
		FireProcess();

		if (INPUT.KeyDown(VK_RBUTTON))
			StartRoll();

		if (flagFireCamShake)
			ShakeCam(timeFireCamShake);

		Reloading();
		Hitting();
	}

	void Player::Walk()
	{
		Unit::SetTarget(weapons[curWeaponIdx]);
		Move();
		FireProcess();

		if (INPUT.KeyDown(VK_RBUTTON))
			StartRoll();

		if (flagFireCamShake)
			ShakeCam(timeFireCamShake);

		Reloading();
		Hitting();
		Dusting();

		if (TIMER.GetTick(timeWalkSound, intervalWalkSound))
		{
			SOUND.Stop("FootstepStone");
			SOUND.Play("FootstepStone");
		}
	}

	void Player::Roll()
	{
		timeRoll += DELTA;
		col->MoveWorldPos(moveDir * (scalar * 1.5f) * cos(timeRoll / 0.63f * DIV2PI) * DELTA);

		if (timeRoll > 0.63f)
		{
			StartIdle();
			state = State::idle;

			weapons[curWeaponIdx]->idle->isVisible = true;

			godMode = false;

			SOUND.Play("Roll");
		}

		Reloading();
		Hitting();
		Dusting();
	}

	void Player::Die()
	{
		Unit::Die();
		for (auto& elem : dust) 
			elem->idle->isVisible = false;
		weapons[curWeaponIdx]->idle->isVisible = false;
		weapons[curWeaponIdx]->imgReloading->isVisible = false;
	}

	void Player::Fall()
	{
		if (TIMER.GetTick(timeRespawnEnd, intervalFallEnd))
		{
			fall->isVisible = false;

			// 가까운 지형 찾기
			bool flagLoopBreak = false;
			for (int i = 1; i <= 2 && false == flagLoopBreak; i++)
			{
				for (int j = 0; j < 8 && false == flagLoopBreak; j++)
				{
					int nx = On().x + dx[j] * i;
					int ny = On().y + dy[j] * i;
					TileState nState = MAPINFO.tilemap->GetTileState(Vec2i(nx, ny));
					switch (nState)
					{
					case TileState::floor:
					case TileState::prop:
					case TileState::spawner:
						Vector2 wpos = MAPINFO.tilemap->TileIdxToWorldPos(Vec2i(nx, ny));
						wpos.x += MAPINFO.tilemap->scale.x / 2.0f;
						wpos.y += MAPINFO.tilemap->scale.x / 2.0f;
						SetPos(wpos);
						Update();
						flagLoopBreak = true;
						break;
					}
				}
			}

			StartRespawn();
		}
	}

	void Player::Respawn()
	{
		Hitting();
		if (TIMER.GetTick(timeRespawnEnd, intervalRespawnEnd))
		{
			respawn->isVisible = false;
			if (curHp <= 0)
				StartDie();
			else
				StartIdle();
		}
	}

	void Player::Cinematic()
	{
	}

	void Player::Move()
	{
		if (INPUT.KeyPress('A'))
			moveDir.x = static_cast<float>(dx[DirState::dirL]);
		else if (INPUT.KeyPress('D'))
			moveDir.x = static_cast<float>(dx[DirState::dirR]);
		else
			moveDir.x = static_cast<float>(dx[DirState::dirNone]);

		if (INPUT.KeyPress('W'))
			moveDir.y = static_cast<float>(dy[DirState::dirT]);
		else if (INPUT.KeyPress('S'))
			moveDir.y = static_cast<float>(dy[DirState::dirB]);
		else
			moveDir.y = static_cast<float>(dy[DirState::dirNone]);

		if (false == (moveDir.x == 0.0f && moveDir.y == 0.0f))
		{
			moveDir.Normalize();
			col->MoveWorldPos(moveDir * scalar * DELTA);
		}
	}

	void Player::FireProcess()
	{
		int firstFire = weapons[curWeaponIdx]->bulletCount - 1;

		if (curBulletIdx == firstFire ||
			TIMER.GetTick(timeFire, weapons[curWeaponIdx]->intervalFire))
		{
			if (curBulletIdx < 0)
			{
				if (weapons[curWeaponIdx]->remainBulletCount > 0)
				{
					isReloading = true;

					for (auto& elem : canFireOnce) elem = false;

					uiReload->img->isVisible = true;
					uiReloadBar->img->isVisible = true;

					SOUND.Play("Reload");
				}
			}

			if ((INPUT.KeyPress(VK_LBUTTON) || INPUT.KeyDown(VK_LBUTTON)))
				Fire();
		}
	}

	void Player::Fire()
	{
		if (false == isReloading)
		{
			canFireOnce[(int)weapons[curWeaponIdx]->type] = true;
			flagFireCamShake = true;
		}

		weapons[curWeaponIdx]->idle->ChangeAnim(AnimState::once, 0.2f);
		flagLbutton = true;

		if (curBulletIdx < 0)
			SOUND.Play("EmptyBullet");
		else
		{
			switch (weapons[curWeaponIdx]->type)
			{
			case WeaponType::pistol:

				if (canFireOnce[(int)WeaponType::pistol])
				{
					weapons[curWeaponIdx]->fireEffect->Spawn(weapons[curWeaponIdx]->firePos->GetWorldPos());
					weapons[curWeaponIdx]->uiBullet[curBulletIdx]->img->isVisible = false;

					bullet[curBulletIdx]->Spawn(
						weapons[curWeaponIdx]->firePos->GetWorldPos(),
						Vector2(RANDOM.Float(targetDir.x - 0.1f, targetDir.x + 0.1f),
							RANDOM.Float(targetDir.y - 0.1f, targetDir.y + 0.1f))
					);

					SOUND.Stop("Pistol");
					SOUND.Play("Pistol");

					canFireOnce[(int)WeaponType::pistol] = false;

					curBulletIdx--;
				}
				break;

			case WeaponType::shotgun:

				if (canFireOnce[(int)WeaponType::shotgun])
				{
					weapons[curWeaponIdx]->fireEffect->Spawn(weapons[curWeaponIdx]->firePos->GetWorldPos());
					weapons[curWeaponIdx]->uiBullet[curBulletIdx]->img->isVisible = false;

					for (int bulletOnce = 6; bulletOnce >= 0; bulletOnce--)
					{
						bullet[bulletOnce]->scalar = RANDOM.Float(800.0f, 1000.0f);

						bullet[bulletOnce]->Spawn(
							weapons[curWeaponIdx]->firePos->GetWorldPos(),
							Vector2(RANDOM.Float(targetDir.x - 0.3f, targetDir.x + 0.3f),
								RANDOM.Float(targetDir.y - 0.3f, targetDir.y + 0.3f))
						);
					}

					SOUND.Stop("Shotgun");
					SOUND.Play("Shotgun");

					canFireOnce[(int)WeaponType::shotgun] = false;

					curBulletIdx--;
				}
				break;

			case WeaponType::machineGun:

				if (canFireOnce[(int)WeaponType::machineGun])
				{
					weapons[curWeaponIdx]->fireEffect->Spawn(weapons[curWeaponIdx]->firePos->GetWorldPos());
					weapons[curWeaponIdx]->uiBullet[curBulletIdx]->img->isVisible = false;

					bullet[curBulletIdx]->Spawn(
						weapons[curWeaponIdx]->firePos->GetWorldPos(),
						Vector2(RANDOM.Float(targetDir.x - 0.1f, targetDir.x + 0.1f),
							RANDOM.Float(targetDir.y - 0.1f, targetDir.y + 0.1f))
					);

					SOUND.Stop("MachineGun");
					SOUND.Play("MachineGun");

					canFireOnce[(int)WeaponType::machineGun] = false;

					curBulletIdx--;
				}
				break;
			}
		}
	}
	
	// 발사 시, 피격 시
	void Player::ShakeCam(float& time)
	{
		CAM.position = Vector2(RANDOM.Float(CAM.position.x - 2.0f, CAM.position.x + 2.0f),
			RANDOM.Float(CAM.position.y - 2.0f, CAM.position.y + 2.0f));

		if (TIMER.GetTick(time, 0.2f))
			flagFireCamShake = false;
	}

	void Player::StartWalk()
	{
		Unit::StartWalk();
		roll->isVisible = false;
		respawn->isVisible = false;
	}

	void Player::StartIdle()
	{
		Unit::StartIdle();
		roll->isVisible = false;
		respawn->isVisible = false;
		weapons[curWeaponIdx]->idle->isVisible = true;
	}

	void Player::StartRoll()
	{
		if (false == (moveDir.x == 0.0f && moveDir.y == 0.0f))
		{
			state = State::roll;

			idle->isVisible = false;
			walk->isVisible = false;
			weapons[curWeaponIdx]->idle->isVisible = false;
			weapons[curWeaponIdx]->imgReloading->isVisible = false;

			SetDirState(moveDir, curMoveDirState);
			roll->frame.y = curMoveDirState;
			roll->isVisible = true;
			roll->ChangeAnim(AnimState::once, intervalAnim[(int)State::roll]);

			timeRoll = 0.0f;

			godMode = true;
		}
	}

	void Player::StartFall()
	{
		idle->isVisible = false;
		walk->isVisible = false;
		roll->isVisible = false;
		weapons[curWeaponIdx]->idle->isVisible = false;
		weapons[curWeaponIdx]->imgReloading->isVisible = false;
		shadow->isVisible = false;

		fall->isVisible = true;
		fall->ChangeAnim(AnimState::once, intervalAnim[(int)State::fall]);

		StartHit(1);

		state = State::fall;

		SOUND.Play("Fall");
	}

	void Player::StartRespawn()
	{
		idle->isVisible = false;
		walk->isVisible = false;
		roll->isVisible = false;

		weapons[curWeaponIdx]->idle->isVisible = true;
		shadow->isVisible = true;
		respawn->isVisible = true;
		respawn->ChangeAnim(AnimState::once, intervalAnim[(int)State::respawn]);

		state = State::respawn;

		SOUND.Play("Respawn");
	}

	void Player::StartDie()
	{
		Unit::StartDie();

		roll->isVisible = false;
		weapons[curWeaponIdx]->idle->isVisible = false;
		
		die->ChangeAnim(AnimState::once, intervalAnim[(int)State::die]);

		DecreaseHeart();

		SOUND.Play("Death");
	}

	void Player::Reloading()
	{
		if (isReloading)
		{
			uiReloadBar->img->MoveLocalPos(Vector2(80.0f * DELTA, 0.0f));
			weapons[curWeaponIdx]->idle->isVisible = false;
			weapons[curWeaponIdx]->imgReloading->isVisible = true;

			if (TIMER.GetTick(timeReload, 1.5f))
			{
				if (weapons[curWeaponIdx]->remainBulletCount == INT_MAX)
					curBulletIdx = weapons[curWeaponIdx]->bulletCount - 1;
				else
				{
					int tempRemain = weapons[curWeaponIdx]->remainBulletCount;
					weapons[curWeaponIdx]->remainBulletCount -=
						min(weapons[curWeaponIdx]->bulletCount - curBulletIdx - 1,
							weapons[curWeaponIdx]->remainBulletCount);
					curBulletIdx = min(weapons[curWeaponIdx]->bulletCount - 1, tempRemain);
				}

				int idx = 0;
				for (auto& elem : weapons[curWeaponIdx]->uiBullet)
				{
					if (idx > curBulletIdx) break;
					elem->img->isVisible = true;
					idx++;
				}

				weapons[curWeaponIdx]->idle->isVisible = true;
				weapons[curWeaponIdx]->imgReloading->isVisible = false;
				uiReload->img->isVisible = false;
				uiReloadBar->img->isVisible = false;
				isReloading = false;
				uiReloadBar->img->SetLocalPosX(-60.0f);
			}
		}
	}

	void Player::Hitting()
	{
		if (isHit)
		{
			DecreaseHeart();

			if (TIMER.GetTick(timeHit, 1.0f))
				isHit = false;
		}

		if (isHitAnim)
		{
			Color c = Color(RANDOM.Float(0.6f, 1.0f), 0.5f, 0.5f, RANDOM.Float(0.2f, 1.0f));
			idle->color = c;
			walk->color = c;
			roll->color = c;

			ShakeCam(timeHitCamShake);

			if (TIMER.GetTick(timeHitAnim, 1.0f))
			{
				Color c = Color(0.5f, 0.5f, 0.5f, 1.0f);
				idle->color = c;
				walk->color = c;
				roll->color = c;

				isHitAnim = false;
			}
		}
		else
		{
			idle->color.w = 1.0f;
			walk->color.w = 1.0f;
		}
	}

	void Player::Dusting()
	{
		if (TIMER.GetTick(timeLastPosForDust, 0.6f))
		{
			dust[curDustIdx++]->Spawn(colTile->GetWorldPos());
			if (curDustIdx >= dustMax) curDustIdx = 0;
			SOUND.Stop("Dust");
			SOUND.Play("Dust");
		}
	}

	void Player::DecreaseHeart()
	{
		if (curHp & 1)
			uiHeartFull[curHp / 2]->img->isVisible = false;
		else
			uiHeartHalf[curHp / 2]->img->isVisible = false;
	}

	void Player::PlusMoney(const int n)
	{
		money += n;
		SOUND.Play("Coin");
	}

	void Player::EquipWeapon(shared_ptr<Weapon> other)
	{
		// 이전 무기 끄기
		shared_ptr<Weapon>& beforeWeapon = weapons[curWeaponIdx];
		beforeWeapon->idle->isVisible = false;
		beforeWeapon->firePos->isVisible = false;
		beforeWeapon->imgReloading->isVisible = false;
		for (auto& elem : beforeWeapon->uiBullet)
			elem->img->isVisible = false;
		if (beforeWeapon->uiWeapon)
			beforeWeapon->uiWeapon->img->isVisible = false;
		if (beforeWeapon->uiBulletFrame)
			beforeWeapon->uiBulletFrame->img->isVisible = false;

		curWeaponIdx++;

		// 새 무기
		shared_ptr<Weapon>& afterWeapon = weapons[curWeaponIdx];

		switch (other->type)
		{
		case WeaponType::pistol:
			afterWeapon = make_shared<Weapon1>();
			break;
		case WeaponType::shotgun:
			afterWeapon = make_shared<Weapon2>();
			break;
		case WeaponType::machineGun:
			afterWeapon = make_shared<Weapon3>();
			break;
		default:
			break;
		}

		afterWeapon->col->SetParentRT(*col);
		afterWeapon->col->SetLocalPos(afterWeapon->localPosDefault);
		afterWeapon->Equip();

		// bullet
		curBulletIdx = afterWeapon->bulletCount - 1;
		bullet.resize(afterWeapon->bulletCount);
		for (auto& elem : bullet)
			if (!elem)
				elem = make_shared<PlayerBullet>();

		// UI
		for (auto& elem : weapons[curWeaponIdx]->uiBullet)
			elem->img->isVisible = true;
		weapons[curWeaponIdx]->uiBulletFrame->img->isVisible = true;
		weapons[curWeaponIdx]->uiWeapon->img->isVisible = true;

		SOUND.Play("WeaponPickup");
	}

	void Player::SetWeaponFrameToOrigin()
	{
		if (flagLbutton && TIMER.GetTick(timeWeaponFrameToOrigin, 0.2f))
		{
			flagLbutton = false;
			weapons[curWeaponIdx]->idle->frame.x = 0;
			weapons[curWeaponIdx]->idle->ChangeAnim(AnimState::stop, 0.1f);
		}
	}

	void Player::UIOn(const bool on)
	{
		isUIRendering = on;
		weapons[curWeaponIdx]->UIOn(on);
	}

	void Player::ColToggle()
	{
		Character::ColToggle();
		weapons[curWeaponIdx]->col->isVisible ^= 1;
		weapons[curWeaponIdx]->firePos->isVisible ^= 1;
		for (auto& bulletElem : bullet)
			bulletElem->col->isVisible ^= 1;
	}
}