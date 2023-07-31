#pragma once

namespace Gungeon
{
	enum class CinematicState
	{
		none,
		// ���� ����
		cinematicBox1,
		cameraTargeting1,
		bossSpawnAnim,
		cutScene,
		cameraTargeting2,
		cinematicBox2,
		finish,
		// ���� ���
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
		void ResizeScreen() const;

		void BoxUp(const bool isUp) const;

	public:
		CinematicState cinematicState = CinematicState::none;
		std::shared_ptr<UI> box[2];

	private:
		float timeCinematic[static_cast<int>(CinematicState::max)];
		float intervalCinematic[static_cast<int>(CinematicState::max)];
	};
}
