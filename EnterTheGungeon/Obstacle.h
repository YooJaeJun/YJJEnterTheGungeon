#pragma once

namespace Gungeon
{
	class Obstacle : public Character
	{
	public:
		Obstacle();
		void Update() override;
		void LateUpdate() override;
		void Render() override;

	public:
		std::shared_ptr<ObImage>	idle = nullptr;
	};
}