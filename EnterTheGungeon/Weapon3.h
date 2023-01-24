#pragma once

namespace Gungeon
{
	class Weapon3 : public Weapon
	{
	public:
		Weapon3();
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;
	};
}