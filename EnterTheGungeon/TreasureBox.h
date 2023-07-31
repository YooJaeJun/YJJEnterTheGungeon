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

	constexpr int treasureWeaponNum = 2;

	class TreasureBox final : public Obstacle
	{
	public:
		TreasureBox();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		void Spawn(const Vector2 wpos) override;

	public:
		TreasureState	treasureState = TreasureState::none;
		std::shared_ptr<Weapon>			weapon[treasureWeaponNum];

	private:
		std::shared_ptr<ObImage>		open = nullptr;
		Vector2			weaponDest[treasureWeaponNum];
	};
}
