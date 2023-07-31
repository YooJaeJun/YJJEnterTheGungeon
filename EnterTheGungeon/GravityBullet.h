#pragma once

namespace Gungeon
{
	class GravityBullet final : public Bullet
	{
	public:
		GravityBullet();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Spawn(const Vector2& coord, const Vector2& fireDir) override;
	};
}