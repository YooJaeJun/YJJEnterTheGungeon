#pragma once
// ���߿� ��������Ʈ �̺�Ʈ��...
class Sound
{
    struct SoundNode
    {
        FMOD::Sound* sound;        //����� ���� ������
        FMOD::Channel* channel;    //������� ä��
    };
    map<string, SoundNode*> SoundList;
    FMOD::System* system;

private:
    Sound();

public:
    static Sound& GetInstance()
    {
        static std::unique_ptr<Sound> inst;
        if (!inst)
            inst = std::unique_ptr<Sound>(new Sound());
        return *inst;
    }

public:
    ~Sound();
    //�������� ���� �߰� 
    bool AddSound(const string& File, const string& Key, const bool loop = false);
    //�������� ���� ����
    bool DeleteSound(const string& Key);

    //�׸��� key�� �Ű������� �ΰ� ��� ����
    void Play(const string& Key);
    void Stop(const string& Key);
    //�Ͻ�����
    void Pause(const string& Key);
    //�Ͻ����� ����
    void Resume(const string& Key);

    void SetVolume(const string& Key, const float scale);
    void SetMasterVolume();
    void Update() const;
};

