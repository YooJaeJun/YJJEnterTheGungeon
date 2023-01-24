#pragma once

namespace Gungeon
{
	class Effect : public Character
	{
	public:
		Effect();
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void Spawn(const Vector2 wpos) override;
		void Idle();
		void Die();

	public:
		std::shared_ptr<ObImage>	idle = nullptr;
		float		intervalDie = 0.5f;

	private:
		float		timeDie = 0.0f;
	};
}