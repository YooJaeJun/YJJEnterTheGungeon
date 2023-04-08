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

	class Enemy3 : public Enemy
	{
	public:
		Enemy3();
		virtual ~Enemy3() {}
		virtual void Init() override;
		void InitVar();
		void InitSelf();
		void InitWeapon();
		void InitBullet();
		virtual void InitIntervalAttack() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Fire() override;
		virtual void StartDie() override;

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