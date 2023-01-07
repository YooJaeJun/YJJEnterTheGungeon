#pragma once

namespace Gungeon
{
	enum class WeaponType
	{
		pistol,
		shotgun,
		machineGun,
		max
	};

	class Weapon : public Item
	{
	public:
		Weapon();
		virtual void Release() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;

		void Equip();
		void EquipRight();
		void EquipLeft();
		void UIOn(const bool on);

	private:
		void SwapUvY();

	public:
		WeaponType	type = WeaponType::pistol;
		GameObject* firePos = nullptr;
		Effect*		fireEffect = nullptr;
		ObImage*	imgReloading = nullptr;
		UI*			uiBulletFrame = nullptr;
		vector<UI*>	uiBullet;
		UI*			uiWeapon = nullptr;
		int			bulletCount = 0;
		UI*			uiBulletCountInfinity = nullptr;
		int			remainBulletCount = 0;
		float		intervalFire;
		Vector2		pivotDefault;
		Vector2		localPosDefault;
		Vector2		localFirePosDefault;
		Vector2		uiWeaponSpawnPos;

	private:
		float		timeFireEffect = 0.0f;
		Vector2		uiBulletCountInfinityPos;
		bool		isUIRendering = true;
	};
}