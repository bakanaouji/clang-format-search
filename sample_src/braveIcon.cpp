//==========================================================
/// @file
/// @brief    braveIcon.h�̎���
/// @author   �������V
///
/// @attention  �E�҂̃A�C�R���摜��\���N���X�ł��B

//==========================================================

#include "braveIcon.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
BraveIcon::BraveIcon() : Image()
{
	mSpriteData.width = braveIconNS::WIDTH;			// �E�҂̃A�C�R���̃T�C�Y
	mSpriteData.height = braveIconNS::HEIGHT;
	mSpriteData.x = braveIconNS::X;					// ��ʏ�̈ʒu
	mSpriteData.y = braveIconNS::Y;
	mSpriteData.rect.bottom = braveIconNS::HEIGHT;	// ��ʂ̈ꕔ��I��
	mSpriteData.rect.right = braveIconNS::WIDTH;
	mStartFrame = braveIconNS::START_FRAME;			// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = braveIconNS::END_FRAME;				// �A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;						// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
}

//=============================================================================
// Update
// �v���C���[�̎c��̗͂ɉ����ăA�j���[�V�������X�V����B
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================	
void BraveIcon::update(const float frameTime)
{
	// �v���C���[�̗̑͂ɉ����āA�A�j���[�V�����𕪊�
	if (mCurrentFrame == mStartFrame && mBrave->getHealth() < 75)
		setCurrentFrame(mStartFrame + 1);
	if (mCurrentFrame == mStartFrame + 1 && mBrave->getHealth() < 50)
		setCurrentFrame(mStartFrame + 2);
	if (mCurrentFrame == mStartFrame + 2 && mBrave->getHealth() < 25)
		setCurrentFrame(mStartFrame + 3);
}

//==========================================================
// �p�����[�^���Z�b�g
//==========================================================
void BraveIcon::reset()
{
	// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
	setCurrentFrame(mStartFrame);
}
