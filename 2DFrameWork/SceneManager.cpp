#include "framework.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
    scenes.clear();
}

bool SceneManager::AddScene(string key, std::shared_ptr<Scene> value)
{
    auto iter = scenes.find(key);

    if (iter != scenes.end())
    {
        return false;
    }
    scenes[key] = value;

    return true;
}

bool SceneManager::DeleteScene(string key)
{
    auto iter = scenes.find(key);

    if (iter == scenes.end())
    {
        return false;
    }

    scenes.erase(iter);

    return true;
}

shared_ptr<Scene> SceneManager::ChangeScene(string key, float changingTime)
{
    shared_ptr<Scene> temp = GetScene(key);

    if (temp)
    {
        nextScene = temp;
        this->changingTime = changingTime;

        if (changingTime <= 0.0f)
        {
            isChanging = true;
            //¾À ºÒ·¯¿Ã ¶§ 
            currentScene.reset();
            //¾À ¹Ù²ð ¶§
            nextScene->Init();
        } 
    }
    return temp;
}

shared_ptr<Scene> SceneManager::GetScene(string key)
{
    auto iter = scenes.find(key);

    if (iter == scenes.end())
    {
        return nullptr;
    }

    return iter->second;
}

shared_ptr<Scene> SceneManager::GetCurrentScene()
{
    return currentScene;
}

void SceneManager::Update()
{
    if (changingTime > 0.0f)
    {
        changingTime -= DELTA;

        if (changingTime <= 0.0f)
        {
            isChanging = true;
            currentScene = nullptr;
            nextScene->Init();
        }
    }


    if (isChanging)
    {
        currentScene = nextScene;
        isChanging = false;
    }
    currentScene->Update();
}

void SceneManager::LateUpdate()
{
    if (isChanging)return;
    currentScene->LateUpdate();
}

void SceneManager::Render()
{
    if (isChanging)return;
    currentScene->Render();
}

void SceneManager::ResizeScreen()
{
    if (isChanging || !currentScene)return;
    currentScene->ResizeScreen();
}
