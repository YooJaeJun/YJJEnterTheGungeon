#pragma once

namespace Gungeon
{
	class Scene01 final : public Scene
	{
	public:
		Scene01() = default;
		~Scene01() override = default;

		void Init() override;
		void Update() override;
		void LateUpdate() override {}
		void Render() override;
		void ResizeScreen() override {}

		void ChangeScene1() const;
		void ChangeScene2();
		void ChangeUpdateScene();

	public:
		std::shared_ptr<ProcedureMapGeneration> mapGen = nullptr;
		std::shared_ptr<Player>					player = nullptr;

	private:
		bool	isChangingScene = false;
		float	timeFade = 0.0f;
	};
}