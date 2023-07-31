#pragma once

namespace Gungeon
{
	enum class Enemy3FireState
	{
		none,
		z,
		circle,
		target,
		toWalk
	};

	class Enemy3 final : public Enemy
	{
	public:
		Enemy3();
		~Enemy3() override = default;
		void Init() override;
		void InitVar() override;
		void InitSelf() override;
		void InitWeapon();
		void InitBullet();
		void InitIntervalAttack() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;
		void Fire() override;
		void StartDie() override;

	private:
		Enemy3FireState		fireState = Enemy3FireState::none;
		const int			bulletMax = 43;
		float				timeBulletTarget = 0.0f;
		Vector2				spawnPos;
		float				timeBulletZ = 0.0f;
		float				timeBulletCircle = 0.0f;
		float				timeAttackToWalk = 0.0f;
		int					curBulletIdx = 0;
		int					curBulletX = 0;
		int					curBulletY = 0;
		StringBulletData	stringBullet;
	};
}