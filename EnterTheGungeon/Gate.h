#pragma once

namespace Gungeon
{
	enum class GateState
	{
		none,
		opening,
		open,
		cinematic,
		process,
		setting,
		set,
		closing,
		closed
	};

	class Gate final : public Obstacle
	{
	public:
		Gate();
		void Init() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;
		void Spawn(const Vector2 wpos) override;

	public:
		bool		flagIntersectPlayer = false;
		GateState	gateState = GateState::none;
		bool		flagPlayerDisappear = false;
		Vector2		playerDest;

	private:
		std::shared_ptr<ObImage>	bottom = nullptr;
		float		timeOpen = 0.0f;
		float		timeProcess = 0.0f;
		float		timeCinematicBox = 0.0f;
		float		timeSet = 0.0f;
		float		timeClosing = 0.0f;
		float		timePlayerDisappear = 0.0f;
		float		timeClosed = 0.0f;
	};
}
