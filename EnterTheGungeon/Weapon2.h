#pragma once

namespace Gungeon
{
	class Weapon2 : public Weapon
	{
	public:
		Weapon2();
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;
	};
}