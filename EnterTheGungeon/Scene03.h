#pragma once

namespace Gungeon
{
	class Scene03 : public Scene
	{
	public:
		Scene03();
		~Scene03();

		virtual void Init() override;
		void InitRoom();
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;
		void IntersectPlayer();
		void IntersectBoss();
		void CinematicProcess();
		void UIOn(const bool on);
		void ChangeUpdateScene();
		void ChangeScene3();
		void ColToggle();

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