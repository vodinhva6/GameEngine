#include "AudioManager.h"

MyAudioEngine::~MyAudioEngine()
{
    clear();
    delete dxtkAudio;
}

void MyAudioEngine::musicPause(int trackNo)
{
    dxtkAudio->musicPause(trackNo);
}

void MyAudioEngine::musicResume(int trackNo)
{
    dxtkAudio->musicResume(trackNo);
}

MyAudioEngine::MyAudioEngine()
{
   
    
    //AudioSpace::sound::load(MY_BGM, L"./Data/Audio/WaveBank.xwb", 0.5f);
    //music::load(MY_TITLE, L"./Data/Asset/Musics/title.wav", 0.5f);
    //music::load(MY_TUTO, L"./Data/Asset/Musics/tuto.wav", 0.5f);
}
void MyAudioEngine::Init()
{
    dxtkAudio = new audio::DXTKAudio;
    //loadMusic(GAMEBGM, L"./Data/Asset/Musics/game.wav", 0.5f);
    //loadMusic(MAINBGM, L"./Data/Asset/Musics/title.wav", 0.5f);
    //loadEffect(EFFECT, L"./Data/Audio/effect.xwb", 0.5f);
    
    //loadMusic(GAMEBGM, L"./Data/Asset/Musics/gameBGM.wav", 0);
    //loadMusic(MAINBGM, L"./Data/Asset/Musics/mainBGM.wav", 0);
    //loadMusic(WINGAME, L"./Data/Asset/Musics/win.wav", 0);
    //loadMusic(CHARRUN, L"./Data/Asset/Sounds/run.wav", 0.3f);
    //loadMusic(CHARWALK, L"./Data/Asset/Sounds/walk.wav", 0.3f);


    //loadEffect(EFFECT_1, L"./Data/Asset/Sounds/this.xwb", 0.5f);
    

}

void MyAudioEngine::loadEffect(int xwbNo, const wchar_t* xwbFileName, float volume)
{
    dxtkAudio->soundLoad(xwbNo, xwbFileName, volume);
}

void MyAudioEngine::play(int xwbNo, int trackNo)
{
    // XWB‚ª“Ç‚Ýž‚Ü‚ê‚Ä‚¢‚È‚¯‚ê‚ÎƒŠƒ^[ƒ“
    if (!dxtkAudio->xwbIsInUse(xwbNo))
    {
        assert(!"xwb‚ª“Ç‚Ýž‚Ü‚ê‚Ä‚¢‚È‚¢");
        return;
    }

    dxtkAudio->soundPlay(xwbNo, trackNo);
}

void MyAudioEngine::update()
{
    dxtkAudio->update();
}


void MyAudioEngine::loadMusic(int trackNo, const wchar_t* waveFileName, float volume)
{
    dxtkAudio->musicLoad(trackNo, waveFileName, volume);
}


void MyAudioEngine::playMusic(int trackNo, bool isLoop)
{
    dxtkAudio->musicPlay(trackNo, isLoop);
}


void MyAudioEngine::stopAll()
{
    for (int i = 0; i < MUSIC_FILE_MAX; i++)
    {
        stop(i);
    }
}


void MyAudioEngine::stop(int trackNo)
{
    dxtkAudio->musicStop(trackNo);
}