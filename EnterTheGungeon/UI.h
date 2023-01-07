#pragma once

namespace Gungeon
{
	enum class UIType
	{
		none,
		prop,
		gauge,
		font
	};

	class UI
	{
	public:
		UI();

		void Release();
		void Update();
		void LateUpdate();
		void Render();

		void Spawn(const Vector2 factor);
		bool DownGauge();

	public:
		ObImage*	img = nullptr;
		DirState	anchor = DirState::dirNone;
		Vector2		imgSize;
	};
}
