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
		void Release();
		void Update();
		void LateUpdate();
		void Render();
		void Hit();

	public:
		ObImage*	idle = nullptr;
		bool		flagAbsorbed = false;
		Vector2		targetPos;
	};
}
