#pragma once

namespace Gungeon
{
	class Obstacle : public Character
	{
	public:
		Obstacle();
		void Release();
		void Update();
		void LateUpdate();
		void Render();

	public:
		ObImage*	idle = nullptr;
	};
}