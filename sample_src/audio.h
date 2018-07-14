//==========================================================
/// @file
/// @brief    Audio�N���X
/// @author   �������V
///
/// @attention  �I�[�f�B�I���Đ��A��~����N���X�ł��B

//==========================================================
#ifndef _AUDIO_H                // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _AUDIO_H                // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================
#include <xact3.h>
#include "constants.h"

//==========================================================
// �I�[�f�B�I���Đ��A��~����N���X�ł��B
class Audio
{
    // Audio�v���p�e�B
  private:
    IXACT3Engine* mXactEngine;   // XACT�G���W���ւ̃|�C���^
    IXACT3WaveBank* mWaveBank;   // XACT�E�F�C�u�o���N�ւ̃|�C���^
    IXACT3SoundBank* mSoundBank; // XACT�T�E���h�o���N�ւ̃|�C���^
    XACTINDEX mCueI;             // XACT�T�E���h�C���f�b�N�X
    void* mMapWaveBank;          // UnmapViewOfFile()���Ăяo���ăt�@�C�������
    void* mSoundBankData;
    bool mCoInitialized;         // CoInitializeEx�����������ꍇ�Atrue��ݒ�

  public:
	// �R���X�g���N�^
    Audio();

    // �f�X�g���N�^
    virtual ~Audio();

    // �����o�[�֐�
    // Audio��������
	// �߂�l�F�������������������ǂ���
    HRESULT initialize();

    // �T�E���h�G���W���̎����I�^�X�N�����s
    void run();

	// �L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N����Đ�
	// �L���[�����݂��Ȃ��ꍇ�́A�T�E���h���Đ�����Ȃ������ŁA�G���[�͔������Ȃ�
    // �����Fcue	�Đ�����T�E���h�̖��O
	void playCue(const char cue[]);

	// �L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N�Œ�~
	// �L���[�����݂��Ȃ��ꍇ�A�G���[�͔������Ȃ�
	// �����Fcue	��~����T�E���h�̖��O
    void stopCue(const char cue[]);
};

#endif
