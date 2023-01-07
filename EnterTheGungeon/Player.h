#pragma once

namespace Gungeon
{
	const int weaponMax = 3;
	const int dustMax = 2;

	class Player : public Unit
	{
	public:
		Player();
		virtual void Init() override;
		void InitVar();
		void InitCol();
		void InitAnim();
		void InitWeapon();
		void InitBullet();
		void InitEffect();
		void InitUI();
		virtual void Release() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;
		void SetTargetAndCamera();
		void Idle();
		void Walk();
		void Roll();
		void Fall();
		void Respawn();
		virtual void Die() override;
		void Cinematic();
		void Move();
		void FireProcess();
		void Fire();
		void ShakeCam(float& time);
		virtual void StartWalk() override;
		virtual void StartIdle() override;
		void StartRoll();
		void StartFall();
		void StartRespawn();
		virtual void StartDie() override;
		void Reloading();
		void Hitting();
		void Dusting();
		void DecreaseHeart();
		void PlusMoney(const int n);
		void EquipWeapon(Weapon* other);
		void SetWeaponFrameToOrigin();
		void UIOn(const bool on);
		virtual void ColToggle() override;

	public:
		ObImage*			roll = nullptr;
		Weapon*				weapons[weaponMax];
		bool				flagInteractionUI = false;
		bool				godMode = false;
		int					curWeaponIdx = 0;

	private:
		int					curWeaponMax = 0;
		bool				canFireOnce[(int)WeaponType::max];
		bool				isReloading = false;
		float				timeReload = 0.0f;
		int					curBulletIdx = 0;
		float				timeRoll = 0.0f;
		bool				flagFireCamShake = false;
		float				timeFireCamShake = 0.0f;
		float				timeHitCamShake = 0.0f;
		UI*					uiReload = nullptr;
		UI*					uiReloadBar = nullptr;
		bool				godModeByForce = false;
		float				timeLastPosForDust = 0.0f;
		Effect*				dust[dustMax];
		int					curDustIdx = 0;
		UI*					uiWeaponFrame = nullptr;
		vector<UI*>			uiHeartFull;
		vector<UI*>			uiHeartHalf;
		vector<UI*>			uiHeartNone;
		vector<UI*>			uiBlank;
		UI*					uiKey = nullptr;
		UI*					uiGold = nullptr;
		int					money = 0;
		int					key = 0;
		bool				flagLbutton = false;
		float				timeWeaponFrameToOrigin = 0.0f;
		float				timeFallEnd = 0.0f;
		float				intervalFallEnd = 1.2f;
		float				timeRespawnEnd = 0.0f;
		float				intervalRespawnEnd = 0.7f;
		float				timeWalkSound = 0.0f;
		float				intervalWalkSound = 0.3f;
		bool				isUIRendering = true;
	};
}