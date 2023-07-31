#pragma once

namespace Gungeon
{
	class Door final : public Obstacle
	{
	public:
		Door();
		void Init() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		void Spawn(const Vector2 wpos, const DirState dir);
		void Disappear();
	};
}
