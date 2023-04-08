#pragma once

namespace Gungeon
{
	enum class MapGenState 
	{ 
		none,
		spray, 
		spread, 
		select, 
		triangulate, 
		span, 
		loop, 
		clean, 
		roomTile, 
		passageTile, 
		passagePitTile, 
		prop,
		spawner,
		propPit,
		propWall,
		finish,
		max
	};

	// B, L, R, LB, RB, T, LT, RT
	const Int2 wallImgDir[8] = { {1, 3}, {0, 1}, {3, 1}, {0, 3}, {3, 3}, {1, 0}, {0, 0}, {3, 0} };
	const Int2 floorImgMin = { 1, 1 };
	const Int2 floorImgMax = { 2, 2 };

	class ProcedureMapGeneration
	{
	public:
		ProcedureMapGeneration();

		void Init();
		void Clear();
		void Update();
		void LateUpdate();
		void Render();
		void ResizeScreen();

		void Spray();
		void Spread();
		void Select();
		void Triangulate();
		void Spanning();
		void Loop();
		void Clean();
		void RoomTile();
		void PassageTile();
		void PassagePitTile();
		void Prop();
		void Spawner();
		void PropPit();
		void PropWall();
		void Finish();

		void SetTileRange(const TileState tileState, const Int2 imgMin, const Int2 imgMax, 
			const Int2 sour, const Int2 dest, const int roomIdx);
		void SetTileSpecificDir(const TileState tileState, const Int2 imgDir[], 
			const Int2 on, const DirState dir, const int roomIdx);
		void SetTileAllDir(const TileState tileState, const Int2 imgDir[], 
			const Int2 sour, const Int2 dest, const int roomIdx);
		void VisualizeSpawner();
		void SetGateSpawner();
		void SetEnemySpawner();
		void SetTreasureSpawner();
		// void MaximalSquare(const Room* elem);
		void Histogram(const shared_ptr<Room> elem);
		void Save();
		void Load();

	private:
		const float timeDefault = 0.5f;
		const int roomMax = 12;
		const int gridMax = 7000;
		const Int2 passagePitImgMin = { 0, 4 };
		const Int2 passagePitImgMax = { 3, 4 };
		const Int2 propImgMin = { 0, 5 };
		const Int2 propImgMax = { 4, 5 };
		const Int2 spawnerImg = { 5, 5 };
		// B, L, R, LB, RB, T, LT, RT
		const Int2 propWallImgDir[8] = { {1, 9}, {0, 7}, {3, 7}, {0, 9}, {3, 9}, {1, 6}, {0, 6}, {3, 6} };
		const Int2 propWallFloorImgMin = { 1, 7 };
		const Int2 propWallFloorImgMax = { 2, 8 };
		const Int2 propPitImgDir[8] = { {5, 9}, {4, 7}, {7, 7}, {4, 9}, {7, 9}, {5, 6}, {4, 6}, {7, 6} };
		const Int2 propPitFloorImgMin = { 5, 7 };
		const Int2 propPitFloorImgMax = { 6, 8 };

	public:
		std::vector<std::shared_ptr<Room>>	selectedRooms;

	private:
		// 동적할당하지 않은 변수들은 보여주기 용도
		bool								autoProcess = false;
		float								timer = 0.0f;
		// Room
		std::vector<std::shared_ptr<Room>>	candidateRooms;
		MapGenState							state = MapGenState::none;
		MapGenState							finalState = MapGenState::none;
		float								roomScaleForSelect = 0.0f;
		// Delaunay Triangulation
		std::vector<ObNode>					roomNode;
		Delaunay							triangulation;
		// MST - Prim
		std::vector<ObLine>					linesTriangulated;
		std::vector<ObLine>					linesMST;
		// Passage
		std::map<ObNode, int>				nodesForRoomIndex;
		std::vector<Tile>					way;
		std::vector<vector<Int2>>			passages;
		string								file;
		// propWall, propPit
		int									areaMax = 0;
		int									xMax = 0;
		int									yMax = 0;
		Int2								propSour, propDest;
	};
}