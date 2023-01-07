#pragma once

namespace Gungeon
{
	class Enemy2 : public Enemy
	{
	public:
		Enemy2();
		virtual void Init() override;
		void InitVar();
		void InitSelf();
		void InitWeapon();
		void InitBullet();
		virtual void InitIntervalAttack() override;
		virtual void Release() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Fire() override;

	private:
		const int	bulletMax = 25;
		int			curBulletIdx = 0;
	};
}