#include "stdafx.h"
#include "Main.h"
using namespace Gungeon;

void Main::Init()
{
    LIGHT.light.radius = 2000.0f;
    LIGHT.light.select = 0.0f;

    InitSound();

    mapGen = make_shared<ProcedureMapGeneration>();
    player = make_shared<Player>();

    {
        shared_ptr<Scene01> tempScene = make_shared<Scene01>();
        tempScene->mapGen = mapGen;
        tempScene->player = player;
        SCENE.AddScene("Scene01", tempScene);
    }

    SCENE.ChangeScene("Scene01");
}

void Gungeon::Main::InitSound()
{
    SOUND.AddSound("EnterTheGungeon/BGM_MapGen.ogg", "BGM_MapGen", true);
    SOUND.AddSound("EnterTheGungeon/BGM_Game.ogg", "BGM_Game", true);
    SOUND.AddSound("EnterTheGungeon/BGM_Boss.ogg", "BGM_Boss", true);
    SOUND.AddSound("EnterTheGungeon/Roll.ogg", "Roll", false);
    SOUND.AddSound("EnterTheGungeon/Fall.ogg", "Fall", false);
    SOUND.AddSound("EnterTheGungeon/Respawn.ogg", "Respawn", false);
    SOUND.AddSound("EnterTheGungeon/Death.ogg", "Death", false);
    SOUND.AddSound("EnterTheGungeon/Pistol.ogg", "Pistol", false);
    SOUND.AddSound("EnterTheGungeon/Shotgun.ogg", "Shotgun", false);
    SOUND.AddSound("EnterTheGungeon/MachineGun.ogg", "MachineGun", false);
    SOUND.AddSound("EnterTheGungeon/Reload.ogg", "Reload", false);
    SOUND.AddSound("EnterTheGungeon/EmptyBullet.ogg", "EmptyBullet", false);
    SOUND.AddSound("EnterTheGungeon/Bomb.ogg", "Bomb", false);
    SOUND.AddSound("EnterTheGungeon/Dust.ogg", "Dust", false);
    SOUND.AddSound("EnterTheGungeon/Coin.ogg", "Coin", false);
    SOUND.AddSound("EnterTheGungeon/FootstepStone.ogg", "FootstepStone", false);
    SOUND.AddSound("EnterTheGungeon/BookCharge.ogg", "BookCharge", false);
    SOUND.AddSound("EnterTheGungeon/BookAttack.ogg", "BookAttack", false);
    SOUND.AddSound("EnterTheGungeon/BookDrop.ogg", "BookDrop", false);
    SOUND.AddSound("EnterTheGungeon/EnemySpawning.ogg", "EnemySpawning", false);
    SOUND.AddSound("EnterTheGungeon/EnemySpawn.ogg", "EnemySpawn", false);
    SOUND.AddSound("EnterTheGungeon/BulletManDeath.ogg", "BulletManDeath", false);
    SOUND.AddSound("EnterTheGungeon/BulletManHurt.ogg", "BulletManHurt", false);
    SOUND.AddSound("EnterTheGungeon/BossIntro.ogg", "BossIntro", false);
    SOUND.AddSound("EnterTheGungeon/BulletKingChairDown.ogg", "BulletKingChairDown", false);
    SOUND.AddSound("EnterTheGungeon/BulletKingShot.ogg", "BulletKingShot", false);
    SOUND.AddSound("EnterTheGungeon/BulletKingThrow.ogg", "BulletKingThrow", false);
    SOUND.AddSound("EnterTheGungeon/BulletKingSpin.ogg", "BulletKingSpin", false);
    SOUND.AddSound("EnterTheGungeon/Voice_Boss_Spawn.ogg", "Voice_Boss_Spawn", false);
    SOUND.AddSound("EnterTheGungeon/Voice_Boss_Tornado.ogg", "Voice_Boss_Tornado", false);
    SOUND.AddSound("EnterTheGungeon/Voice_Boss_Hit1.ogg", "Voice_Boss_Hit1", false);
    SOUND.AddSound("EnterTheGungeon/Voice_Boss_Hit2.ogg", "Voice_Boss_Hit2", false);
    SOUND.AddSound("EnterTheGungeon/Voice_Boss_Die.ogg", "Voice_Boss_Die", false);
    SOUND.AddSound("EnterTheGungeon/Voice_Boss_RealDie.ogg", "Voice_Boss_RealDie", false);
    SOUND.AddSound("EnterTheGungeon/SpawnByForce.ogg", "SpawnByForce", false);
    SOUND.AddSound("EnterTheGungeon/FinalShot.ogg", "FinalShot", false);
    SOUND.AddSound("EnterTheGungeon/Win.ogg", "Win", false);
    SOUND.AddSound("EnterTheGungeon/ChestUnlock.ogg", "ChestUnlock", false);
    SOUND.AddSound("EnterTheGungeon/ChestOpen.ogg", "ChestOpen", false);
    SOUND.AddSound("EnterTheGungeon/WeaponPickup.ogg", "WeaponPickup", false);
    SOUND.AddSound("EnterTheGungeon/GateSlam.ogg", "GateSlam", false);
    SOUND.AddSound("EnterTheGungeon/GateOpen.ogg", "GateOpen", false);
    SOUND.AddSound("EnterTheGungeon/DoorClosed.ogg", "DoorClosed", false);
}

void Main::Update()
{
    SCENE.Update();
}

void Main::LateUpdate()
{
    SCENE.LateUpdate();
}

void Main::Render()
{
    SCENE.Render();
}

void Main::ResizeScreen()
{
    SCENE.ResizeScreen();
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
    app.SetAppName(L"Enter the Gungeon by ¿Ø¿Á¡ÿ");
    app.SetInstance(instance);
    app.InitWidthHeight(1400.0f, 740.0f);
    app.background = Color(0.0f, 0.0f, 0.0f, 1.0f);
    shared_ptr<Main> main = make_shared<Main>();
    int wParam = (int)WIN.Run(main);
    return wParam;
}
