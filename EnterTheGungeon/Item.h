#pragma once

namespace Gungeon
{
	enum class ItemType
	{
		none,
		money,
		weapon,
	};

	class Item : public Character
	{
	public:
		Item();
		void Update();
		void LateUpdate();
		void Render();
		void Hit();

	public:
		std::shared_ptr<ObImage>	idle = nullptr;
		bool		flagAbsorbed = false;
		Vector2		targetPos;
	};
}
