#pragma once

namespace Gungeon
{
	class Weapon3 final : public Weapon
	{
	public:
		Weapon3();
		void Update() override;
		void Render() override;
		void ResizeScreen() override;
	};
}