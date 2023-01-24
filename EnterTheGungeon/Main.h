#pragma once

namespace Gungeon
{
	class Main : public Scene
	{
	public:
		virtual void Init() override;
		void InitSound();
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;

	private:
		std::shared_ptr<ProcedureMapGeneration> mapGen = nullptr;
		std::shared_ptr<Player>					player = nullptr;
	};
}