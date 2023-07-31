#pragma once

namespace Gungeon
{
	class Main final : public Scene
	{
	public:
		void Init() override;
		static void InitSound();
		void Update() override;
		void LateUpdate() override;
		void Render() override;
		void ResizeScreen() override;

	private:
		std::shared_ptr<ProcedureMapGeneration> mapGen = nullptr;
		std::shared_ptr<Player>					player = nullptr;
	};
}