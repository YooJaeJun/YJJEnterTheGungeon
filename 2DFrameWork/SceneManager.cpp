#include "framework.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
    scenes.clear();
}

bool SceneManager::AddScene(const string& key, const std::shared_ptr<Scene> value)
{
    // If Init Statement
	if (const auto iter = scenes.find(key); iter != scenes.end())
        return false;

    scenes[key] = value;

    return true;
}

bool SceneManager::DeleteScene(const string& key)
{
	const auto iter = scenes.find(key);

    if (iter == scenes.end())
        return false;

    scenes.erase(iter);

    return true;
}

shared_ptr<Scene> SceneManager::ChangeScene(const string& key, const float changingTime)
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

shared_ptr<Scene> SceneManager::GetScene(const string& key)
{
    auto iter = scenes.find(key);

    if (iter == scenes.end())
        return nullptr;

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

void SceneManager::LateUpdate() const
{
    if (isChanging)
        return;
    currentScene->LateUpdate();
}

void SceneManager::Render() const
{
    if (isChanging)
        return;
    currentScene->Render();
}

void SceneManager::ResizeScreen() const
{
    if (isChanging || !currentScene)
        return;
    currentScene->ResizeScreen();
}
