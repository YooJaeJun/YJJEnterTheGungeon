#pragma once

namespace Gungeon
{
	class Weapon1 final : public Weapon
	{
	public:
		Weapon1();
		void Update() override;
		void Render() override;
		void ResizeScreen() override;
	};
}