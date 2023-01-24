#pragma once

namespace Gungeon
{
	class Bullet : public Character
	{
	public:
		Bullet();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Update(const bool notRotation) override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void Spawn(const Vector2& coord);
		virtual void Spawn(const Vector2& coord, const Vector2& fireDir);
		void Fire();
		void Hit(const float damage);
		virtual void Reload();

	public:
		std::shared_ptr<ObImage>	idle = nullptr;
		std::shared_ptr<Effect>		hitBomb = nullptr;
		bool		isFired = false;
		Vector2		moveDir;
		int			damage = 1;
		float		angle = 0.0f;
		float		atkAngle = 0.0f;
		bool		increaseState = true;
		float		gravity = 0.0f;
		float		moveFactor = 1.0f;

	private:
		float		timeRespawn = 0.0f;
		float		timeLife = 0.0f;
		float		intervalLife = 10.0f;
	};
}