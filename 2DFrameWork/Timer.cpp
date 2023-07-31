#include "framework.h"

Timer::Timer()
{
    //���� �ð� ����
    lastTime = chrono::steady_clock::now();
    //Ÿ�̸� �����ÿ� ����ð��� �˼�����.
    deltaTime = 0.0f;
    //ó�� ����ð��� 0
    worldTime = 0.0f;
    framePerSecond = 0;
    framePerSecondCount = 0;
    framePerSecondTimeElapsed = 0.0f;
}

bool Timer::GetTick(float& time, const float interval)
{
    time += DELTA;
    if (time > interval)
    {
        time -= interval;
        return true;
    }
    return false;
}

void Timer::Chronometry(const UINT lock)
{
    currentTime = chrono::steady_clock::now();
    //�����ð� - ����ð����� ���� �ʴ����� �ޱ�
    chrono::duration<double> Delta = currentTime - lastTime;
    //        �ʴ����� �����Ͽ� ���ޱ�
    deltaTime = static_cast<float>(Delta.count());

    //��Ÿ�� 1/60���� �۴ٸ�
    while (deltaTime < (1.0f / lock))
    {
        //����ð��� �ٽ�����
        currentTime = chrono::steady_clock::now();
        //��Ÿ�� �ٽð��
        Delta = currentTime - lastTime;
        deltaTime = static_cast<float>(Delta.count());
    }

    //������ �ð��� ���
    lastTime = currentTime;
    //��ü �ð� ������� ����
    worldTime += deltaTime;

    //1�ʸ� ����ִ� ����
    framePerSecondTimeElapsed += deltaTime;

    framePerSecondCount++;
    //FPSTimeElapsed �� 1�ʰ� �Ѿ�����
    if (framePerSecondTimeElapsed > 1.0f)
    {
        framePerSecond = framePerSecondCount;
        framePerSecondCount = 0;
        framePerSecondTimeElapsed = 0.0f;
    }
    deltaScaleTime = deltaTime * app.deltaScale;
}

void Timer::CheckStartTimer()
{
    startTimer = currentTime;
}

float Timer::CheckEndTimer() const
{
	const std::chrono::duration<double> diff = chrono::steady_clock::now() - startTimer;
    return static_cast<float>(diff.count());
}

void Timer::DebugPrintTimer(const string& str)
{
    printf("%15s %15f \n", str.c_str(), TIMER.CheckEndTimer());
}

void Timer::Update() const
{
    ImGui::Text("FPS : %d", GetFramePerSecond());
    ImGui::Text("World Time : %f", GetWorldTime());
    ImGui::Text("Delta Time : %f", GetDeltaTime());
}
