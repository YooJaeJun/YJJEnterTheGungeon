#pragma once

namespace Gungeon
{
	enum class RoomType
	{
		start,
		treasure,
		enemy,
		boss,
	};

	constexpr int enemySpawnPosMax = 4;

	class Room final : public Character
	{
	public:
		Room();
		void Init() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;

		[[nodiscard]] Vec2i TileLB() const;
		[[nodiscard]] Vec2i TileLT() const;
		[[nodiscard]] Vec2i TileRB() const;
		[[nodiscard]] Vec2i TileRT() const;
		[[nodiscard]] int TileWidth() const;
		[[nodiscard]] int TileHeight() const;

	public:
		bool				selected = false;
		std::shared_ptr<ObCircle>		enemySpawner[enemySpawnPosMax];
		std::shared_ptr<ObCircle>		treasureSpawner;
		std::shared_ptr<ObCircle>		gateSpawner[9];
		bool				cleared = false;
		RoomType			roomType = RoomType::enemy;
		std::vector<Vec2i>	doorTileIdxs{};
	};
}
