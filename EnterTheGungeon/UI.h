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
		void Update() const;
		void Render() const;

		void Spawn(const Vector2 factor) const;
		bool DownGauge() const;

	public:
		std::shared_ptr<ObImage>	img = nullptr;
		DirState	anchor = DirState::dirNone;
		Vector2		imgSize;
	};
}
