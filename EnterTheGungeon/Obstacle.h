#pragma once

namespace Gungeon
{
	class Obstacle : public Character
	{
	public:
		Obstacle();
		void Update();
		void LateUpdate();
		void Render();

	public:
		std::shared_ptr<ObImage>	idle = nullptr;
	};
}