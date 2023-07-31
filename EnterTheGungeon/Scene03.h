#pragma once

namespace Gungeon
{
	class Scene03 : public Scene
	{
	public:
		Scene03() = default;
		~Scene03() override = default;

		void Init() override;
		void InitRoom();

		void Update() override;
		void LateUpdate() override;
		void Render() override;
		void ResizeScreen() override;

		void IntersectPlayer() const;
		void IntersectBoss() const;
		void CinematicProcess();
		void UIOn(const bool on) const;
		void ChangeUpdateScene();
		void ChangeScene3();
		void ColToggle() const;

	public:
		std::shared_ptr<Player>		player = nullptr;

	private:
		std::shared_ptr<Room>		curRoom = nullptr;
		std::shared_ptr<Boss>		boss = nullptr;
		std::shared_ptr<Cinematic>	cinematic = nullptr;
		bool						isChangingScene = false;
		float						timeFade = 0.0f;
	};
}