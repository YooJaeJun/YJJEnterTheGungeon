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

	const int enemySpawnPosMax = 4;

	class Room : public Character
	{
	public:
		Room();
		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		Int2 TileLB() const;
		Int2 TileLT() const;
		Int2 TileRB() const;
		Int2 TileRT() const;
		int TileWidth() const;
		int TileHeight() const;

	public:
		bool				selected = false;
		std::shared_ptr<ObCircle>		enemySpawner[enemySpawnPosMax];
		std::shared_ptr<ObCircle>		treasureSpawner;
		std::shared_ptr<ObCircle>		gateSpawner[9];
		bool				cleared = false;
		RoomType			roomType = RoomType::enemy;
		std::vector<Int2>	doorTileIdxs;
	};
}
