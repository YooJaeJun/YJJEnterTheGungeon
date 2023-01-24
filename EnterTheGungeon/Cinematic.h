#pragma once

namespace Gungeon
{
	enum class CinematicState
	{
		none,
		// 보스 스폰
		cinematicBox1,
		cameraTargeting1,
		bossSpawnAnim,
		cutScene,
		cameraTargeting2,
		cinematicBox2,
		finish,
		// 보스 사망
		cinematicBox3,
		cameraTargeting3,
		bossDieAnim,
		cameraTargeting4,
		cinematicBox4,
		finish2,
		clear,
		max
	};

	class Cinematic
	{
	public:
		Cinematic();
		void Init();
		void Update();
		void Render();
		void ResizeScreen();

		void BoxUp(const bool isUp);

	public:
		CinematicState cinematicState = CinematicState::none;
		std::shared_ptr<UI> box[2];

	private:
		float timeCinematic[(int)CinematicState::max];
		float intervalCinematic[(int)CinematicState::max];
	};
}
