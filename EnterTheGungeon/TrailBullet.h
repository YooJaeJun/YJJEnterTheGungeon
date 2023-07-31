#pragma once

namespace Gungeon
{
	class TrailBullet : public Bullet
	{
	public:
		TrailBullet();
		void Init() override;
		void Update() override;
		void Render() override;

	private:
		std::deque<std::shared_ptr<ObImage>>	trails;
		float		timeTrail;
		float		timeSpawnTrail;
		float		trailDuration;
		int			trailNum;
	};
}