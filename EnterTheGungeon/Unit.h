#pragma once

namespace Gungeon
{
	class Unit : public Character
	{
	public:
		Unit();
		virtual void Init() override;
		virtual void Release() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;

		virtual void SetTarget(Weapon*& weapon);
		virtual void Idle();
		virtual void Walk();
		virtual void Attack();
		virtual void StartHit(const int damage);
		virtual void Die();
		virtual void StartWalk();
		virtual void StartIdle();
		virtual void StartAttack();
		virtual void StartDie();
		virtual void Spawn(const Vector2 wpos) override;
		void SetLastDir();
		void SetDirState(const Vector2 dir, DirState& dirState);
		void FindPath(ObTileMap* map);
		void Stop();

	public:
		ObImage*			respawn = nullptr;
		ObImage*			shadow = nullptr;
		Item*				dropItem = nullptr;
		vector<Bullet*>		bullet;
		ObImage*			idle = nullptr;
		ObImage*			walk = nullptr;
		Vector2				targetPos;
		wstring				name;
		wstring				desc;

	protected:
		ObImage*			kick = nullptr;
		ObImage*			hit = nullptr;
		ObImage*			fall = nullptr;
		ObImage*			die = nullptr;
		ObImage*			attack = nullptr;
		ObImage*			attackEnd = nullptr;
		DirState			curMoveDirState = DirState::dirB;
		DirState			curMoveDirStateBefore = DirState::dirB;
		DirState			curTargetDirState = DirState::dirB;
		DirState			curTargetDirStateBefore = DirState::dirB;
		Vector2				targetDir;
		Vector2				targetDirBefore;
		Vector2				originCamPos;
		float				intervalAnim[(int)State::max];
		float				intervalHit = 0.0f;
		float				timeFire = 0.0f;
		float				timeReload = 0.0f;
		Vector2				pushedDir;
		float				timeHit = 0.0f;
		bool				isHit = false;
		bool				isHitAnim = false;
		float				timeHitAnim = 0.0f;
		float				timeDieAnim = 0.0f;
		float				timeRealDie = 0.0f;
		float				timeAttackStart = 0.0f;
		float				intervalAttackStart = 0.0f;
		float				timeFindPath = 0.0f;
		float				timeTarget = 0.0f;
		vector<Tile>		way;

	private:
		float				g = 0.0f;	// ∫Ò¿≤
		Vector2				start;
		Vector2				end;
	};
}