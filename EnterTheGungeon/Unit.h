#pragma once

namespace Gungeon
{
	class Unit : public Character
	{
	public:
		Unit();
		~Unit() override = default;
		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;

		virtual void SetTarget(std::shared_ptr<Weapon>& weapon);
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
		static void SetDirState(const Vector2 dir, DirState& dirState);
		void FindPath(const std::shared_ptr<ObTileMap> map);
		void Stop();

	public:
		std::shared_ptr<ObImage>			respawn = nullptr;
		std::shared_ptr<ObImage>			shadow = nullptr;
		std::shared_ptr<Item>				dropItem = nullptr;
		vector<std::shared_ptr<Bullet>>		bullet;
		std::shared_ptr<ObImage>			idle = nullptr;
		std::shared_ptr<ObImage>			walk = nullptr;
		Vector2				targetPos;
		wstring				name;
		wstring				desc;

	protected:
		std::shared_ptr<ObImage>			kick = nullptr;
		std::shared_ptr<ObImage>			hit = nullptr;
		std::shared_ptr<ObImage>			fall = nullptr;
		std::shared_ptr<ObImage>			die = nullptr;
		std::shared_ptr<ObImage>			attack = nullptr;
		std::shared_ptr<ObImage>			attackEnd = nullptr;
		DirState			curMoveDirState = DirState::dirB;
		DirState			curMoveDirStateBefore = DirState::dirB;
		DirState			curTargetDirState = DirState::dirB;
		DirState			curTargetDirStateBefore = DirState::dirB;
		Vector2				targetDir;
		Vector2				targetDirBefore;
		Vector2				originCamPos;
		float				intervalAnim[static_cast<int>(State::max)];
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