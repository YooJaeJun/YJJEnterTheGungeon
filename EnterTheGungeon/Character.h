#pragma once

namespace Gungeon
{
	enum class State
	{
		idle,
		walk,
		roll,
		attack,
		fall,
		respawn,
		die,
		cinematic,
		max
	};

	class Character
	{
	public:
		Character();
		virtual void Init();
		virtual void Update();
		virtual void Update(const bool notRotation);
		virtual void LateUpdate();
		virtual void Render();
		virtual void ResizeScreen();

		inline Int2 On() const { return on; }
		inline Vector2 Pos() const { return col->GetWorldPos(); }
		inline void SetPos(const Vector2 pos) { col->SetWorldPos(pos); }
		inline void SetPosX(const float x) { col->SetWorldPosX(x); }
		inline void SetPosY(const float y) { col->SetWorldPosY(y); }
		virtual void Spawn(const Vector2 wpos);
		void StepBack();
		void SetLastPos();
		virtual void ColToggle();

	public:
		std::shared_ptr<GameObject>	col = nullptr;
		std::shared_ptr<ObRect>		colTile = nullptr;
		State		state = State::idle;
		float		scalar = 0.0f;

	protected:
		Vector2		moveDir;
		Vector2		lastPos;
		int			curHp = 0;
		int			maxHp = 0;
		bool		isHit = false;

	private:
		Int2		on = Int2(0, 0);
	};
}