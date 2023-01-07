#pragma once

namespace Gungeon
{
	enum class TreasureState
	{
		none,
		spawn,
		opening,
		dropping,
		finish
	};

	const int treasureWeaponNum = 2;

	class TreasureBox : public Obstacle
	{
	public:
		TreasureBox();
		virtual void Init() override;
		virtual void Release() override;
		virtual void Update() override;
		virtual void Render() override;
		void Spawn(const Vector2 wpos);

	public:
		TreasureState	treasureState = TreasureState::none;
		Weapon*			weapon[treasureWeaponNum];

	private:
		ObImage*		open = nullptr;
		Vector2			weaponDest[treasureWeaponNum];
	};
}
