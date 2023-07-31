#pragma once
// 나중에 델리게이트 이벤트로...
class Sound
{
    struct SoundNode
    {
        FMOD::Sound* sound;        //출력할 사운드 데이터
        FMOD::Channel* channel;    //출력해줄 채널
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
    //전역에서 사운드 추가 
    bool AddSound(const string& File, const string& Key, const bool loop = false);
    //전역에서 사운드 삭제
    bool DeleteSound(const string& Key);

    //그리고 key를 매개변수로 두고 모두 제어
    void Play(const string& Key);
    void Stop(const string& Key);
    //일시정지
    void Pause(const string& Key);
    //일시정지 해제
    void Resume(const string& Key);

    void SetVolume(const string& Key, const float scale);
    void SetMasterVolume();
    void Update() const;
};

