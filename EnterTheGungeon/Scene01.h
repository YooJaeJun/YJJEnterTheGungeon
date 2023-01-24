#pragma once

namespace Gungeon
{
	class Scene01 : public Scene
	{
	public:
		Scene01();
		~Scene01();

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;

		void ChangeScene1();
		void ChangeScene2();
		void ChangeUpdateScene();

	public:
		std::shared_ptr<ProcedureMapGeneration> mapGen = nullptr;
		std::shared_ptr<Player>					player = nullptr;

	private:
		bool					isChangingScene = false;
		float					timeFade = 0.0f;
	};
}