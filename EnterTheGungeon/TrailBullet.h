#pragma once

namespace Gungeon
{
	class TrailBullet : public Bullet
	{
	public:
		TrailBullet();
		virtual void Init() override;
		virtual void Release() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		deque<ObImage*>	trails;
		float			timeTrail;
		float			timeSpawnTrail;
		float			trailDuration;
		int				trailNum;
	};
}