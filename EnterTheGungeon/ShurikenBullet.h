#pragma once

namespace Gungeon
{
	enum class ShurikenBulletState
	{
		rotate,
		targeting,
		attack
	};

	class ShurikenBullet : public Bullet
	{
	public:
		ShurikenBullet();
		virtual void Init() override;
		virtual void Update() override;

	public:
		Vector2				target;
		ShurikenBulletState	ShurikenBulletState = ShurikenBulletState::rotate;

	private:
		float				timeTargeting = 0.0f;
		float				intervalTargeting = 3.0f;
		float				timeAttack = 0.0f;
		float				intervalAttack = 0.3f;
		Vector2				wposBeforeRot;
	};
}