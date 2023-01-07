#pragma once

namespace Gungeon
{
	class EnemyBullet : public Bullet
	{
	public:
		EnemyBullet();
		virtual void Init() override;
	};
}