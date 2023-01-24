#pragma once

namespace Gungeon
{
	class Weapon1 : public Weapon
	{
	public:
		Weapon1();
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;
	};
}