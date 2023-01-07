#pragma once

namespace Gungeon
{
	class PlayerBullet : public Bullet
	{
	public:
		PlayerBullet();
		virtual void Init() override;
	};
}