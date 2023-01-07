#pragma once

namespace Gungeon
{
	const int enemyMax = 4;
	const int doorMax = 10;

	enum class GameState
	{
		// 맵 생성 기다리는 상태
		start,
		waitingRoom,
		// 이하 반복되는 상태
		enteringRoom,
		waitingSpawn,
		fight
	};

	class Scene02 : public Scene
	{
	public:
		Scene02();
		~Scene02();

	protected:
		virtual void Init() override;
		virtual void Release() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void ResizeScreen() override;

	private:
		void Start();
		void WaitingRoom();
		void EnteringRoom();
		void WaitingSpawn();
		void Fight();
		void SetCamera();
		void SpawnPlayer();
		void SpawnEffect();
		void SpawnEnemy();
		void SpawnTreasureBox();
		void IntersectPlayer();
		void IntersectEnemy();
		void GateProcess();
		void ChangeUpdateScene();
		void ChangeScene1();
		void ChangeScene2();
		void ChangeScene3();
		void ColToggle();

	public:
		ProcedureMapGeneration* mapGen = nullptr;
		Player*					player = nullptr;

	private:
		Room*					curRoom = nullptr;
		GameState				gameState = GameState::start;
		Enemy*					enemy[enemyMax];
		bool					isChangingScene = false;
		float					timeFade = 0.0f;
		vector<Effect*>			spawnEffect;
		int						afterRoomIdx = -2;
		int						curRoomIdx = 0;
		int						roomClearCount = 0;
		int						roomClearCountForBossBattle = 3;
		Gate*					gate = nullptr;
		Cinematic*				cinematic = nullptr;
		vector<Door*>			door;
		TreasureBox*			treasureBox = nullptr;
	};
}