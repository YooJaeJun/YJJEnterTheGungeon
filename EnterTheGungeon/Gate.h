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

	class Gate : public Obstacle
	{
	public:
		Gate();
		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Spawn(const Vector2 wpos) override;

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
