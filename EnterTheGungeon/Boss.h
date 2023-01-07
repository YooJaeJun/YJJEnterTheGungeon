#pragma once

namespace Gungeon
{
	enum class BossPattern
	{
		none,
		circular,
		string,
		tornado,
		spiral,
		trail,
		miro,
		brute,
		shuriken,
		gravity,
		wave,
		max
	};

	enum class BossAttackState
	{
		none,
		start,
		loop,
		reload,
		end
	};

	class Boss : public Unit
	{
	public:
		Boss();
		virtual void Init() override;
		void InitVar();
		void InitSelf();
		void InitBullet();
		void InitItem();
		virtual void Release() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;

		void Idle();
		void Walk();
		virtual void Attack() override;
		virtual void Die() override;
		void StartHit(const int damage, const Vector2& dir);
		void Hitting();
		virtual void StartAttack() override;
		virtual void StartDie() override;
		virtual void Spawn(const Vector2 wpos) override;
		void HitBullet();
		void SpawnByForceInMiro(const Vector2 wpos);
		void SpawnAnim();
		void SpawnAnimEnd();
		void CutSceneOn();
		void CutSceneOff();
		void UIOn(const bool on);
		virtual void ColToggle() override;

		void ChangePattern(const BossPattern newPattern);
		void InitCircular();
		void InitString();
		void InitTornado();
		void InitSpiral();
		void InitTrail();
		void InitMiro();
		void InitBrute();
		void InitShuriken();
		void InitGravity();
		void InitWave();

		void UpdateBullet();
		void UpdateCircular();
		void UpdateString();
		void UpdateTornado();
		void UpdateSpiral();
		void UpdateTrail();
		void UpdateMiro();
		void UpdateBrute();
		void UpdateShuriken();
		void UpdateGravity();
		void UpdateWave();

	private:
		const int			patternMax = static_cast<int>(BossPattern::max) - 1;
		const int			circularMax = 180;
		const int			tornadoMax = 60;
		const int			spiralMax = 60;
		const int			trailMax = 30;
		const int			miroMax = 75;
		const int			bruteMax = 150;
		const int			miroWidth = 35;
		const int			miroHeight = 35;
		const int			shurikenMax = 40;
		const int			gravityMax = 60;
		const int			waveMax = 500;

	public:
		ObCircle*			firePosTargeting = nullptr;
		BossPattern         pattern = BossPattern::none;
		bool				pushingPlayer = false;
		UI*					cutScene = nullptr;
		bool				realDie = false;

	private:
		ObImage*			attack1Start = nullptr;
		ObImage*			attack1 = nullptr;
		ObImage*			attack2 = nullptr;
		ObImage*			attack3 = nullptr;
		ObImage*			chairIdle = nullptr;
		ObImage*			chairWalk = nullptr;
		ObImage*			chairAttack1Start = nullptr;
		ObImage*			chairAttack1 = nullptr;
		ObImage*			chairAttack2 = nullptr;
		ObImage*			chairAttack3 = nullptr;
		ObImage*			chairDie = nullptr;
		Effect*				dieBomb[4];
		Weapon*				weapon = nullptr;
		ObCircle*			firePosCannon = nullptr;
		UI*					hpGuageBar = nullptr;
		UI*					hpGuage = nullptr;
		StringBulletData    stringBullet;
		Vector2				bulletSpawnPos;
		float				timeSetMoveDir = 0.0f;
		float				timeSetTargetDir = 0.0f;
		int					curBulletIdx = 0;
		float				angleFactor = 0.0f;
		vector<string>		candidateString;
		int					candidateStringCount = 0;
		bool				flagSpiralRespawn = false;
		float				timeTornadoSound = 0.0f;
		float				intervalTornadoSound = 0.5f;
		vector<string>		miro;
		bool				miroStart = false;
		int					curBulletX = 0;
		int					curBulletY = 0;
		float				timeAttackEnd = 0.0f;
		BossAttackState		attackState = BossAttackState::none;
		float				intervalFire[(int)BossPattern::max];
		float				intervalEnd[(int)BossPattern::max];
		Effect*				spawnPlayerByForce = nullptr;
		bool				flagRandomPattern = false;
		float				timeInit = 0.0f;
		float				intervalInit = 1.0f;
		float				timeBombBetween = 0.0f;
		float				intervalBombBetween = 0.3f;
		float				intervalBombAnim = 0.1f;
		bool				isUIRendering = true;
	};
}