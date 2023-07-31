#pragma once

namespace Gungeon
{
	class Enemy1 : public Enemy
	{
	public:
		Enemy1();
		~Enemy1() override = default;
		virtual void Init() override;
		void InitVar() override;
		void InitSelf() override;
		void InitWeapon();
		void InitBullet();
		virtual void InitIntervalAttack() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Fire() override;

	private:
		const int	bulletMax = 5;
		int			curBulletIdx = 0;
	};
}