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
		virtual ~Character() = default;
		Character();
		virtual void Init();
		virtual void Update();
		virtual void Update(const bool notRotation);
		virtual void LateUpdate();
		virtual void Render();
		virtual void ResizeScreen();

		Vec2i On() const { return on; }
		Vector2 Pos() const { return col->GetWorldPos(); }
		void SetPos(const Vector2 pos) const { col->SetWorldPos(pos); }
		void SetPosX(const float x) const { col->SetWorldPosX(x); }
		void SetPosY(const float y) const { col->SetWorldPosY(y); }
		virtual void Spawn(const Vector2 wpos);
		void StepBack() const;
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
		Vec2i		on = Vec2i(0, 0);
	};
}