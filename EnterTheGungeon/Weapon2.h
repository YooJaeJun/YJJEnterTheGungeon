#pragma once

namespace Gungeon
{
	class Weapon2 final : public Weapon
	{
	public:
		Weapon2();
		void Update() override;
		void Render() override;
		void ResizeScreen() override;
	};
}