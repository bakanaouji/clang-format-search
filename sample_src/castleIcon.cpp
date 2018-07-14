//==========================================================
/// @file
/// @brief    castleIcon.h�̎���
/// @author   �������V
///
/// @attention  ��̃A�C�R���摜��\���N���X�ł��B

//==========================================================

#include "castleIcon.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
CastleIcon::CastleIcon() : Image()
{
	mSpriteData.width = castleIconNS::WIDTH;			// ��̃A�C�R���̃T�C�Y
	mSpriteData.height = castleIconNS::HEIGHT;
	mSpriteData.x = castleIconNS::X;					// ��ʏ�̈ʒu
	mSpriteData.y = castleIconNS::Y;
	mSpriteData.rect.bottom = castleIconNS::HEIGHT;	// ��ʂ̈ꕔ��I��
	mSpriteData.rect.right = castleIconNS::WIDTH;
	mStartFrame = castleIconNS::START_FRAME;			// �A�j���[�V�����̍ŏ��̃t���[��
	mCurrentFrame = mStartFrame;						// ���݂̃t���[�����A�j���[�V�����̍ŏ��̃t���[���ɃZ�b�g
}

