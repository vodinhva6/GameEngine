#pragma once
#include "AudioEngine.h"
#include "SingletonPatternTemplate.h"
using namespace AudioSpace;


//#define BMG 0
//#define EFFECT 1
//#define KIRAMOVE 2

#define GAMEBGM 0
#define MAINBGM 2
#define WINGAME 3

#define EFFECT_1 3

#define EFFECT_JUMP 0
#define EFFECT_TELE 1
#define EFFECT_SELECTCLICK 2
#define EFFECT_SELECTMOVE 3
#define EFFECT_TAKEHAZUI 4
#define EFFECT_MOVE 5



class MyAudioEngine : public SingletonPatternTemplate<MyAudioEngine>
{
    friend class SingletonPatternTemplate<MyAudioEngine>;
public:

    void Init();

    void loadEffect(int xwbNo, const wchar_t* xwbFileName, float volume);
    void play(int xwbNo, int trackNo);
    void loadMusic(int trackNo, const wchar_t* waveFileName, float volume);
    void playMusic(int trackNo, bool isLoop);
    void stopAll();



    void stop(int trackNo);
    void unload(int trackNo)
    {
        dxtkAudio->musicUnload(trackNo);
    }
    void pauseAll()
    {
        for (int i = 0; i < MUSIC_FILE_MAX; i++)
        {
            pauseTrackNo(i);
        }
    }

    void resumeAll()
    {
        for (int i = 0; i < MUSIC_FILE_MAX; i++)
        {
            resumeTrackNo(i);
        }
    }

    void pauseTrackNo(int trackNo)
    {
        if (!isInUse(trackNo)) return;
        dxtkAudio->musicPause(trackNo);
    }


    void resumeTrackNo(int trackNo)
    {
        if (!isInUse(trackNo)) return;
        dxtkAudio->musicResume(trackNo);
    }

    void setMusicVolume(int trackNo, float volume)
    {
        if (!isInUse(trackNo)) return;
        dxtkAudio->musicSetVolume(trackNo, volume);
    }

    void setEffectVolume(int xwbNo, int trackNo, float vol)
    {
        if (!effectisInUse(xwbNo)) return;
        dxtkAudio->soundSetVolume(xwbNo, trackNo, vol);
    }

    float getVolume(int trackNo)
    {
        if (!isInUse(trackNo)) return 0.0f;
        return dxtkAudio->musicGetVolume(trackNo);
    }

    void fade(int trackNo, float sec, float volume)
    {
        if (!isInUse(trackNo)) return;
        dxtkAudio->musicFade(trackNo, sec, volume);
    }

    void update();

    void clear()
    {
        stopAll();         // ‰¹Šy‚ðŽ~‚ß‚é

        for (int i = 0; i < MUSIC_FILE_MAX; i++)
        {
            unload(i);  // ‰¹Šy‚ð‰ð•ú‚·‚é
        }
    }

    DirectX::SoundState getState(int trackNo)
    {
        return dxtkAudio->musicGetState(trackNo);
    }

    bool isLooped(int trackNo)
    {
        return dxtkAudio->musicIsLooped(trackNo);
    }

    void setPan(int trackNo, float pan)
    {
        if (!isInUse(trackNo)) return;
        dxtkAudio->musicSetPan(trackNo, pan);
    }

    float getPan(int trackNo)
    {
        if (!isInUse(trackNo)) return 0.0f;
        return dxtkAudio->musicGetPan(trackNo);
    }

    float adjustPan(int trackNo, float add)
    {
        if (!isInUse(trackNo)) return 0.0f;
        return dxtkAudio->musicAdjustPan(trackNo, add);
    }

    void setPitch(int trackNo, float pitch)
    {
        if (!isInUse(trackNo)) return;
        dxtkAudio->musicSetPitch(trackNo, pitch);
    }

    float getPitch(int trackNo)
    {
        if (!isInUse(trackNo)) return 0.0f;
        return dxtkAudio->musicGetPitch(trackNo);
    }

    float adjustPitch(int trackNo, float add)
    {
        if (!isInUse(trackNo)) return 0.0f;
        return dxtkAudio->musicAdjustPitch(trackNo, add);
    }

    const WAVEFORMATEX* getFormat(int trackNo)
    {
        return dxtkAudio->musicGetFormat(trackNo);
    }

    bool isInUse(int trackNo)
    {
        return dxtkAudio->musicIsInUse(trackNo);
    }

    bool effectisInUse(int trackNo)
    {
        return dxtkAudio->xwbIsInUse(trackNo);
    }

    void musicPause(int trackNo);
    void musicResume(int trackNo);



    MyAudioEngine();

    ~MyAudioEngine();
private:
    audio::DXTKAudio* dxtkAudio;

};

