#pragma once

namespace Gungeon
{
	constexpr int weaponMax = 3;
	constexpr int dustMax = 2;

	class Player final : public Unit
	{
	public:
		Player();
		~Player() override = default;
		void Init() override;
		void InitVar();
		void InitCol();
		void InitAnim();
		void InitWeapon();
		void InitBullet();
		void InitEffect();
		void InitUI();
		void Update() override;
		void LateUpdate() override;
		void Render() override;
		void ResizeScreen() override;
		void SetTargetAndCamera();
		void Idle();
		void Walk();
		void Roll();
		void Fall();
		void Respawn();
		void Die() override;
		void Move();
		void FireProcess();
		void Fire();
		void ShakeCam(float& time);
		void StartWalk() override;
		void StartIdle() override;
		void StartRoll();
		void StartFall();
		void StartRespawn();
		void StartDie() override;
		void Reloading();
		void Hitting();
		void Dusting();
		void DecreaseHeart() const;
		void PlusMoney(const int n);
		void EquipWeapon(const std::shared_ptr<Weapon> other);
		void SetWeaponFrameToOrigin();
		void UIOn(const bool on);
		void ColToggle() override;

	public:
		std::shared_ptr<ObImage>	roll = nullptr;
		std::shared_ptr<Weapon>		weapons[weaponMax];
		bool	flagInteractionUI = false;
		bool	godMode = false;
		int		curWeaponIdx = 0;

	private:
		int							curWeaponMax = 0;
		bool						canFireOnce[static_cast<int>(WeaponType::max)];
		bool						isReloading = false;
		float						timeReload = 0.0f;
		int							curBulletIdx = 0;
		float						timeRoll = 0.0f;
		bool						flagFireCamShake = false;
		float						timeFireCamShake = 0.0f;
		float						timeHitCamShake = 0.0f;
		std::shared_ptr<UI>			uiReload = nullptr;
		std::shared_ptr<UI>			uiReloadBar = nullptr;
		bool						godModeByForce = false;
		float						timeLastPosForDust = 0.0f;
		std::shared_ptr<Effect>		dust[dustMax];
		int							curDustIdx = 0;
		std::shared_ptr<UI>			uiWeaponFrame = nullptr;
		vector<std::shared_ptr<UI>>	uiHeartFull;
		vector<std::shared_ptr<UI>>	uiHeartHalf;
		vector<std::shared_ptr<UI>>	uiHeartNone;
		vector<std::shared_ptr<UI>>	uiBlank;
		std::shared_ptr<UI>			uiKey = nullptr;
		std::shared_ptr<UI>			uiGold = nullptr;
		int							money = 0;
		int							key = 0;
		bool						flagLbutton = false;
		float						timeWeaponFrameToOrigin = 0.0f;
		float						timeFallEnd = 0.0f;
		float						intervalFallEnd = 1.2f;
		float						timeRespawnEnd = 0.0f;
		float						intervalRespawnEnd = 0.7f;
		float						timeWalkSound = 0.0f;
		float						intervalWalkSound = 0.3f;
		bool						isUIRendering = true;
	};
}