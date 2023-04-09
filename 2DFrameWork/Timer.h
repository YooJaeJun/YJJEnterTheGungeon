#pragma once
// 나중에 델리게이트 이벤트로...
class Timer
{
private:
    Timer();

public:
    static Timer& GetInstance()
    {
        static std::unique_ptr<Timer> inst;
        if (!inst)
            inst = std::unique_ptr<Timer>(new Timer());
        return *inst;
    }

public:
    bool    GetTick(float& time, float interval);
    void    Chronometry(UINT lock = 60);
    void    CheckStartTimer();
    float   CheckEndTimer();
    void    DebugPrintTimer(const string& str);
    float   GetDelta() { return deltaTime; } //매크로 X
    float   GetDeltaTime() { return deltaScaleTime; }
    UINT    GetFramePerSecond() { return framePerSecond; }
    float   GetWorldTime() { return worldTime; }

    void Update();

private:
    chrono::steady_clock::time_point    currentTime;
    chrono::steady_clock::time_point    lastTime;
    chrono::steady_clock::time_point    startTimer;
    UINT                                framePerSecond;
    UINT                                framePerSecondCount;
    float                               framePerSecondTimeElapsed;
    float                               deltaTime;
    float                               deltaScaleTime;
    float                               worldTime;
};
