#pragma once

namespace Gungeon
{
	class Enemy2 final : public Enemy
	{
	public:
		Enemy2();
		~Enemy2() override = default;
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
		const int	bulletMax = 25;
		int			curBulletIdx = 0;
	};
}