//******************************************************************************
//
//
//      DirectXTKAudio
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include "AudioEngine.h"

namespace AudioSpace
{
    namespace audio
    {
        using DirectX::AUDIO_ENGINE_FLAGS;
        using DirectX::AudioEngine;
        using DirectX::SoundEffect;
        using DirectX::WaveBank;

        using DirectX::AudioEngine_Default;
        using DirectX::AudioEngine_Debug;

        //--------------------------------------------------------------
        //  コンストラクタ
        //--------------------------------------------------------------
        DXTKAudio::DXTKAudio()
        {
            // Create DirectXTK for Audio objects
            AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
            eflags = eflags | AudioEngine_Debug;
#endif
            audioEngine.reset(new AudioEngine(eflags));
//            for (auto &p : soundVolume) p = 1.0f;TODO
        }

        //--------------------------------------------------------------
        //  デストラクタ
        //--------------------------------------------------------------
        DXTKAudio::~DXTKAudio()
        {
            audioEngine.reset();
        }

        //--------------------------------------------------------------
        //  オーディオエンジンのリセット
        //--------------------------------------------------------------
        bool DXTKAudio::reset()
        {
            return audioEngine->Reset();
        }

        //--------------------------------------------------------------
        //  音楽読込
        //--------------------------------------------------------------
        void DXTKAudio::musicLoad(int trackNo, const wchar_t* waveFileName, float volume)
        {
            music[trackNo].load(audioEngine.get(), waveFileName, volume);
            music[trackNo].play(false);
            music[trackNo].stop();
        }

        //--------------------------------------------------------------
        //  音楽解放
        //--------------------------------------------------------------
        void DXTKAudio::musicUnload(int trackNo)
        {
            music[trackNo].unload();
        }

        //--------------------------------------------------------------
        //  音楽再生
        //--------------------------------------------------------------
        void DXTKAudio::musicPlay(int trackNo, bool isLoop)
        {
            music[trackNo].play(isLoop);
        }

        //--------------------------------------------------------------
        //  音楽停止
        //--------------------------------------------------------------
        void DXTKAudio::musicStop(int trackNo)
        {
            music[trackNo].stop();
        }

        //--------------------------------------------------------------
        //  音楽一時停止
        //--------------------------------------------------------------
        void DXTKAudio::musicPause(int trackNo)
        {
            music[trackNo].pause();
        }

        //--------------------------------------------------------------
        //  音楽再開
        //--------------------------------------------------------------
        void DXTKAudio::musicResume(int trackNo)
        {
            music[trackNo].resume();
        }

        //--------------------------------------------------------------
        //  音楽ボリューム設定
        //--------------------------------------------------------------
        void DXTKAudio::musicSetVolume(int trackNo, float volume)
        {
            music[trackNo].setVolume(volume);
        }

        //--------------------------------------------------------------
        //  音楽ボリューム取得
        //--------------------------------------------------------------
        float DXTKAudio::musicGetVolume(int trackNo)
        {
            return music[trackNo].getVolume();
        }


        //--------------------------------------------------------------
        //  音楽のフェードアウト・フェードイン
        //--------------------------------------------------------------
        void DXTKAudio::musicFade(int trackNo, float sec, float volume)
        {
            music[trackNo].fade(sec, volume);
        }

        void DXTKAudio::update()
        {
            for (int i = 0; i < MUSIC_FILE_MAX; i++)
            {
                music[i].update();
            }
        }

        //--------------------------------------------------------------
        //  SoundStateの取得
        //--------------------------------------------------------------
        DirectX::SoundState DXTKAudio::musicGetState(int trackNo)
        {
            return music[trackNo].getState();
        }

        //--------------------------------------------------------------
        //  ループの有無の取得
        //--------------------------------------------------------------
        bool DXTKAudio::musicIsLooped(int trackNo)
        {
            return music[trackNo].isLooped();
        }

        //--------------------------------------------------------------
        //  panの設定
        //--------------------------------------------------------------
        void DXTKAudio::musicSetPan(int trackNo, float pan)
        {
            music[trackNo].setPan(pan);
        }

        //--------------------------------------------------------------
        //  panの取得
        //--------------------------------------------------------------
        float DXTKAudio::musicGetPan(int trackNo)
        {
            return music[trackNo].getPan();
        }

        //--------------------------------------------------------------
        //  panの調整
        //--------------------------------------------------------------
        float DXTKAudio::musicAdjustPan(int trackNo, float pan)
        {
            return music[trackNo].adjustPan(pan);
        }

        //--------------------------------------------------------------
        //  pitchの設定
        //--------------------------------------------------------------
        void DXTKAudio::musicSetPitch(int trackNo, float pitch)
        {
            music[trackNo].setPitch(pitch);
        }

        //--------------------------------------------------------------
        //  pitchの取得
        //--------------------------------------------------------------
        float DXTKAudio::musicGetPitch(int trackNo)
        {
            return music[trackNo].getPitch();
        }

        //--------------------------------------------------------------
        //  pitchの調整
        //--------------------------------------------------------------
        float DXTKAudio::musicAdjustPitch(int trackNo, float pitch)
        {
            return music[trackNo].adjustPitch(pitch);
        }

        //--------------------------------------------------------------
        //  フォーマットの取得
        //--------------------------------------------------------------
        const WAVEFORMATEX* DXTKAudio::musicGetFormat(int trackNo)
        {
            return music[trackNo].getFormat();
        }

        //--------------------------------------------------------------
        //  音楽が使用中かどうか
        //--------------------------------------------------------------
        bool DXTKAudio::musicIsInUse(int trackNo)
        {
            return music[trackNo].isInUse();
        }

        //--------------------------------------------------------------
        //  効果音の読み込み（xwbファイル）
        //--------------------------------------------------------------
        void DXTKAudio::soundLoad(int xwbNo, const wchar_t* xwbFileName, float volume)
        {
            if (xwbNo < 0 || xwbNo >= XWB_FILE_MAX)
            {
                assert(!"xwbインデックスエラー");
                return;
            }

            sound[xwbNo].load(audioEngine.get(), xwbFileName, volume);
        }

        //--------------------------------------------------------------
        //  効果音の再生
        //--------------------------------------------------------------
        void DXTKAudio::soundPlay(int xwbNo, int trackNo)
        {
            sound[xwbNo].play(trackNo);
        }

        //--------------------------------------------------------------
        //  効果音のボリューム設定
        //--------------------------------------------------------------
        void DXTKAudio::soundSetVolume(int xwbNo, int trackNo, float vol)
        {
            if (xwbNo < 0 || xwbNo >= XWB_FILE_MAX)
            {
                assert(!"xwbインデックスエラー");
                return;
            }

            sound[xwbNo].setVolume(trackNo, vol);
        }

        //--------------------------------------------------------------
        //  xwbが有効かどうか
        //--------------------------------------------------------------
        bool DXTKAudio::xwbIsInUse(int xwbNo)
        {
            if (xwbNo < 0 || xwbNo >= XWB_FILE_MAX)
            {
                assert(!"xwbのインデックスの範囲エラー");
                return false;
            }
            return sound[xwbNo].isInUse();
        }
    }
}

//******************************************************************************