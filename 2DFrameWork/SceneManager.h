#pragma once
class SceneManager
{
private:
    SceneManager();

public:
    static SceneManager& GetInstance()
    {
        static std::unique_ptr<SceneManager> inst;
        if (!inst)
            inst = std::unique_ptr<SceneManager>(new SceneManager());
        return *inst;
    }

public:
    ~SceneManager();

    //�� �߰�
    bool    AddScene(const string& key, const std::shared_ptr<Scene> value);
    //�� ����
    bool    DeleteScene(const string& key);
    //�� ����
    std::shared_ptr<Scene> ChangeScene(const string& key, const float changingTime = 0.0f);
    //�� ��������
    std::shared_ptr<Scene> GetScene(const string& key);
    //���� �� ��������
    std::shared_ptr<Scene> GetCurrentScene();

    void    Update();
    void    LateUpdate() const;
    void    Render() const;
    void    ResizeScreen() const;

private:
    bool        isChanging = false;
    std::shared_ptr<Scene>                 nextScene = nullptr;
    std::shared_ptr<Scene>                 currentScene = nullptr;
    map<string, std::shared_ptr<Scene>>    scenes;

    float       changingTime = 0.0f;
};

