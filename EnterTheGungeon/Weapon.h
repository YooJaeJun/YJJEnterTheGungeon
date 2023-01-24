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
		std::shared_ptr<GameObject> firePos = nullptr;
		std::shared_ptr<Effect>		fireEffect = nullptr;
		std::shared_ptr<ObImage>	imgReloading = nullptr;
		std::shared_ptr<UI>			uiBulletFrame = nullptr;
		vector<std::shared_ptr<UI>>	uiBullet;
		std::shared_ptr<UI>			uiWeapon = nullptr;
		int			bulletCount = 0;
		std::shared_ptr<UI>			uiBulletCountInfinity = nullptr;
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