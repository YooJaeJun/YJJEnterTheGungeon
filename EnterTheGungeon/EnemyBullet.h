#pragma once

namespace Gungeon
{
	class EnemyBullet final : public Bullet
	{
	public:
		EnemyBullet();
		virtual void Init() override;
	};
}