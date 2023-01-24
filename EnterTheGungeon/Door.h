#pragma once

namespace Gungeon
{
	class Door : public Obstacle
	{
	public:
		Door();
		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void Spawn(const Vector2 wpos, const DirState dir);
		void Disappear();
	};
}
