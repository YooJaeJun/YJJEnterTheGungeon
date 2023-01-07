#pragma once

namespace Gungeon
{
	class BossBullet : public Bullet
	{
	public:
		BossBullet();
		virtual void Init() override;
	};
}