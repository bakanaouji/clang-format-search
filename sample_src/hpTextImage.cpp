//==========================================================
/// @file
/// @brief    hpTextImage.h�̎���
/// @author   �������V
///
/// @attention  HP�Ƃ��������̉摜��\���N���X�ł��B
///				�V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p�ł��܂��B

//==========================================================
#include "hpTextImage.h"
//==========================================================

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
HpTextImage::HpTextImage() : Image()
{
	mSpriteData.width = hpTextImageNS::WIDTH;		// �e�L�X�g�摜�̃T�C�Y
	mSpriteData.height = hpTextImageNS::HEIGHT;
	mSpriteData.x = hpTextImageNS::X;				// ��ʏ�̏����ʒu
	mSpriteData.y = hpTextImageNS::Y;
	mSpriteData.rect.bottom = hpTextImageNS::HEIGHT;// ��ʂ̈ꕔ��I��
	mSpriteData.rect.right = hpTextImageNS::WIDTH;
	mStartFrame = hpTextImageNS::START_FRAME;		// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = mStartFrame;						// �A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;					// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɃZ�b�g
}