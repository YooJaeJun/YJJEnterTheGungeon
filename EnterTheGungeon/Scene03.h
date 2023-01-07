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
		virtual void Release() override;
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
		Player* player = nullptr;

	private:
		Room*		curRoom = nullptr;
		Boss*		boss = nullptr;
		Cinematic*	cinematic = nullptr;
		bool		isChangingScene = false;
		float		timeFade = 0.0f;
	};
}