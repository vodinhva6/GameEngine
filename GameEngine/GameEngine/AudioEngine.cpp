//******************************************************************************
//
//
//      DirectXTKAudio
//
//
//******************************************************************************

//------< �C���N���[�h >----------------------------------------------------------
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
        //  �R���X�g���N�^
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
        //  �f�X�g���N�^
        //--------------------------------------------------------------
        DXTKAudio::~DXTKAudio()
        {
            audioEngine.reset();
        }

        //--------------------------------------------------------------
        //  �I�[�f�B�I�G���W���̃��Z�b�g
        //--------------------------------------------------------------
        bool DXTKAudio::reset()
        {
            return audioEngine->Reset();
        }

        //--------------------------------------------------------------
        //  ���y�Ǎ�
        //--------------------------------------------------------------
        void DXTKAudio::musicLoad(int trackNo, const wchar_t* waveFileName, float volume)
        {
            music[trackNo].load(audioEngine.get(), waveFileName, volume);
            music[trackNo].play(false);
            music[trackNo].stop();
        }

        //--------------------------------------------------------------
        //  ���y���
        //--------------------------------------------------------------
        void DXTKAudio::musicUnload(int trackNo)
        {
            music[trackNo].unload();
        }

        //--------------------------------------------------------------
        //  ���y�Đ�
        //--------------------------------------------------------------
        void DXTKAudio::musicPlay(int trackNo, bool isLoop)
        {
            music[trackNo].play(isLoop);
        }

        //--------------------------------------------------------------
        //  ���y��~
        //--------------------------------------------------------------
        void DXTKAudio::musicStop(int trackNo)
        {
            music[trackNo].stop();
        }

        //--------------------------------------------------------------
        //  ���y�ꎞ��~
        //--------------------------------------------------------------
        void DXTKAudio::musicPause(int trackNo)
        {
            music[trackNo].pause();
        }

        //--------------------------------------------------------------
        //  ���y�ĊJ
        //--------------------------------------------------------------
        void DXTKAudio::musicResume(int trackNo)
        {
            music[trackNo].resume();
        }

        //--------------------------------------------------------------
        //  ���y�{�����[���ݒ�
        //--------------------------------------------------------------
        void DXTKAudio::musicSetVolume(int trackNo, float volume)
        {
            music[trackNo].setVolume(volume);
        }

        //--------------------------------------------------------------
        //  ���y�{�����[���擾
        //--------------------------------------------------------------
        float DXTKAudio::musicGetVolume(int trackNo)
        {
            return music[trackNo].getVolume();
        }


        //--------------------------------------------------------------
        //  ���y�̃t�F�[�h�A�E�g�E�t�F�[�h�C��
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
        //  SoundState�̎擾
        //--------------------------------------------------------------
        DirectX::SoundState DXTKAudio::musicGetState(int trackNo)
        {
            return music[trackNo].getState();
        }

        //--------------------------------------------------------------
        //  ���[�v�̗L���̎擾
        //--------------------------------------------------------------
        bool DXTKAudio::musicIsLooped(int trackNo)
        {
            return music[trackNo].isLooped();
        }

        //--------------------------------------------------------------
        //  pan�̐ݒ�
        //--------------------------------------------------------------
        void DXTKAudio::musicSetPan(int trackNo, float pan)
        {
            music[trackNo].setPan(pan);
        }

        //--------------------------------------------------------------
        //  pan�̎擾
        //--------------------------------------------------------------
        float DXTKAudio::musicGetPan(int trackNo)
        {
            return music[trackNo].getPan();
        }

        //--------------------------------------------------------------
        //  pan�̒���
        //--------------------------------------------------------------
        float DXTKAudio::musicAdjustPan(int trackNo, float pan)
        {
            return music[trackNo].adjustPan(pan);
        }

        //--------------------------------------------------------------
        //  pitch�̐ݒ�
        //--------------------------------------------------------------
        void DXTKAudio::musicSetPitch(int trackNo, float pitch)
        {
            music[trackNo].setPitch(pitch);
        }

        //--------------------------------------------------------------
        //  pitch�̎擾
        //--------------------------------------------------------------
        float DXTKAudio::musicGetPitch(int trackNo)
        {
            return music[trackNo].getPitch();
        }

        //--------------------------------------------------------------
        //  pitch�̒���
        //--------------------------------------------------------------
        float DXTKAudio::musicAdjustPitch(int trackNo, float pitch)
        {
            return music[trackNo].adjustPitch(pitch);
        }

        //--------------------------------------------------------------
        //  �t�H�[�}�b�g�̎擾
        //--------------------------------------------------------------
        const WAVEFORMATEX* DXTKAudio::musicGetFormat(int trackNo)
        {
            return music[trackNo].getFormat();
        }

        //--------------------------------------------------------------
        //  ���y���g�p�����ǂ���
        //--------------------------------------------------------------
        bool DXTKAudio::musicIsInUse(int trackNo)
        {
            return music[trackNo].isInUse();
        }

        //--------------------------------------------------------------
        //  ���ʉ��̓ǂݍ��݁ixwb�t�@�C���j
        //--------------------------------------------------------------
        void DXTKAudio::soundLoad(int xwbNo, const wchar_t* xwbFileName, float volume)
        {
            if (xwbNo < 0 || xwbNo >= XWB_FILE_MAX)
            {
                assert(!"xwb�C���f�b�N�X�G���[");
                return;
            }

            sound[xwbNo].load(audioEngine.get(), xwbFileName, volume);
        }

        //--------------------------------------------------------------
        //  ���ʉ��̍Đ�
        //--------------------------------------------------------------
        void DXTKAudio::soundPlay(int xwbNo, int trackNo)
        {
            sound[xwbNo].play(trackNo);
        }

        //--------------------------------------------------------------
        //  ���ʉ��̃{�����[���ݒ�
        //--------------------------------------------------------------
        void DXTKAudio::soundSetVolume(int xwbNo, int trackNo, float vol)
        {
            if (xwbNo < 0 || xwbNo >= XWB_FILE_MAX)
            {
                assert(!"xwb�C���f�b�N�X�G���[");
                return;
            }

            sound[xwbNo].setVolume(trackNo, vol);
        }

        //--------------------------------------------------------------
        //  xwb���L�����ǂ���
        //--------------------------------------------------------------
        bool DXTKAudio::xwbIsInUse(int xwbNo)
        {
            if (xwbNo < 0 || xwbNo >= XWB_FILE_MAX)
            {
                assert(!"xwb�̃C���f�b�N�X�͈̔̓G���[");
                return false;
            }
            return sound[xwbNo].isInUse();
        }
    }
}

//******************************************************************************