#pragma once

namespace Gungeon
{
	class Enemy : public Unit
	{
	public:
		Enemy();
		~Enemy() override = default;
		virtual void Init() override;
		virtual void InitVar();
		virtual void InitSelf();
		void InitItem();
		virtual void InitIntervalAttack();
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void Idle() override;
		void Walk() override;
		virtual void Attack() override;
		virtual void Die() override;
		virtual void Fire();
		void StartHit(const int damage, const Vector2& dir);
		void Hitting();
		virtual void StartDie() override;
		virtual void Spawn(const Vector2 wpos) override;
		void AttackAnimStart() const;
		void AttackAnimEnd() const;
		void AttackToWalk() const;
		virtual void ColToggle() override;

	public:
		float		timeSetDir = 0.0f;
		std::shared_ptr<Weapon>		weapon = nullptr;

	private:
		float		timeSetMoveDir = 0.0f;
		float		timeSetTargetDir = 0.0f;
		float		pushedScalar = 400.0f;
		float		timeAiming = 0.0f;
	};
}