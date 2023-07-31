#include "framework.h"

Sound::Sound()
{
    //���� �ý��� ����
    System_Create(&system);
    //���� ä�� �����Ҵ�
    system->init(32, FMOD_INIT_NORMAL, nullptr);
}

Sound::~Sound()
{
    //���带 �� �����ϴ� �κ�
    for (auto iter = SoundList.begin(); iter != SoundList.end(); ++iter)
    {
        iter->second->channel->stop();
        iter->second->sound->release();
        delete iter->second;
    }
    system->release();
    system->close();
    SoundList.clear();
}

bool Sound::AddSound(const string& File, const string& Key, const bool loop)
{
	const string path = "../Contents/Sound/" + File;

    //key �ߺ� ���x

	//�ߺ��Ȱ� �ִ�.
    if (const auto iter = SoundList.find(Key); iter != SoundList.end())
        return false;

    //�ߺ��Ȱ� ����.
    SoundNode* temp = new SoundNode();
    system->createSound
    (
        path.c_str(),
        FMOD_DEFAULT,
        nullptr,
        &temp->sound
    );

    if (loop)
        temp->sound->setMode(FMOD_LOOP_NORMAL);
    else
        temp->sound->setMode(FMOD_LOOP_OFF);

    //�ʿ� �Ҵ��� �迭 ���ҳֱ�
    SoundList[Key] = temp;

    return true;
}

bool Sound::DeleteSound(const string& Key)
{
	const auto iter = SoundList.find(Key);

    //�ߺ��Ȱ� ����.
    if (iter == SoundList.end())
        return false;

    //first�� Ű, second ���
    iter->second->channel->stop();
    iter->second->sound->release();

    //�ʿ����� ����
    SoundList.erase(iter);

    return true;
}

void Sound::Play(const string& Key)
{
	//�ߺ��Ȱ� ������
    if (const auto iter = SoundList.find(Key); iter != SoundList.end())
    {
        bool isplay;
        iter->second->channel->isPlaying(&isplay);
        if (!isplay)
        {
            system->playSound(
                iter->second->sound, nullptr,
                false,
                &iter->second->channel);
        }
    }
}

void Sound::Stop(const string& Key)
{
	//�ߺ��Ȱ� ������
    if (const auto iter = SoundList.find(Key); iter != SoundList.end())
        iter->second->channel->stop();
}

void Sound::Pause(const string& Key)
{
	//�ߺ��Ȱ� ������
    if (const auto iter = SoundList.find(Key); iter != SoundList.end())
        iter->second->channel->setPaused(true);
}

void Sound::Resume(const string& Key)
{
	//�ߺ��Ȱ� ������
    if (const auto iter = SoundList.find(Key); iter != SoundList.end())
        iter->second->channel->setPaused(false);
}

void Sound::SetVolume(const string& Key, const float scale)
{
	//�ߺ��Ȱ� ������
    if (const auto iter = SoundList.find(Key); iter != SoundList.end())
        iter->second->channel->setVolume(scale * app.soundScale);
}

void Sound::SetMasterVolume()
{
    for (auto iter = SoundList.begin(); iter != SoundList.end(); ++iter)
        iter->second->channel->setVolume(app.soundScale);
}

void Sound::Update() const
{
    system->update();
}
