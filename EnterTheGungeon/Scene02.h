#pragma once

namespace Gungeon
{
	constexpr int enemyMax = 4;
	constexpr int doorMax = 10;

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
		~Scene02() override = default;

	protected:
		void Init() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;
		void ResizeScreen() override;

	private:
		void Start();
		void WaitingRoom();
		void EnteringRoom();
		void WaitingSpawn();
		void Fight();
		void SetCamera() const;
		void SpawnPlayer() const;
		void SpawnEffect() const;
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
		std::shared_ptr<ProcedureMapGeneration> mapGen = nullptr;
		std::shared_ptr<Player>					player = nullptr;

	private:
		std::shared_ptr<Room>					curRoom = nullptr;
		GameState								gameState = GameState::start;
		std::shared_ptr<Enemy>					enemy[enemyMax];
		bool									isChangingScene = false;
		float									timeFade = 0.0f;
		vector<std::shared_ptr<Effect>>			spawnEffect;
		int										afterRoomIdx = -2;
		int										curRoomIdx = 0;
		int										roomClearCount = 0;
		int										roomClearCountForBossBattle = 3;
		std::shared_ptr<Gate>					gate = nullptr;
		std::shared_ptr<Cinematic>				cinematic = nullptr;
		std::vector<std::shared_ptr<Door>>		door;
		std::shared_ptr<TreasureBox>			treasureBox = nullptr;
	};
}