//==========================================================
/// @file
/// @brief    hitEffect.h�̎���
/// @author   �������V
///
/// @attention  �U�����q�b�g�����Ƃ��̃A�j���[�V������\������N���X�ł��B

//==========================================================

#include "hitEffect.h"

//==========================================================

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
HitEffect::HitEffect() : Image()
{
	mVisible = false;								// ��\�����f�t�H���g
	mSpriteData.width = hitEffectNS::WIDTH;			// �P�̉摜�̃T�C�Y
	mSpriteData.height = hitEffectNS::HEIGHT;
	mSpriteData.rect.bottom = hitEffectNS::HEIGHT;	// �摜���̑I�����镔��
	mSpriteData.rect.right = hitEffectNS::WIDTH;
	mCols = hitEffectNS::TEXTURE_COLS;				// �e�N�X�`���̗�
	mFrameDelay = hitEffectNS::ANIMATION_DELAY;		// �A�j���[�V�����̊e�t���[���̊Ԋu
	mStartFrame = hitEffectNS::START_FRAME;			// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = hitEffectNS::END_FRAME;				// �A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;						// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
	setScale(0.5);									// �X�P�[��
	mLoop = false;									// �A�j���[�V�����̓��[�v�����Ȃ�
}


//==========================================================
// Update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//==========================================================
void HitEffect::update(const float frameTime)
{
	// ��\�����ɂ͉������Ȃ�
	if (mVisible == false)
		return;
	// �摜���X�V
	Image::update(frameTime);
	// �A�j���[�V�������I�������ꍇ�A��\���ɂ���
	if (mAnimComplete)
		mVisible = false;
}

//==========================================================
// Hit
// ���̊֐����Ă΂��ƍU�����q�b�g�����Ƃ��̃G�t�F�N�g������
//==========================================================
void HitEffect::hit(const float x, const float y)
{
	// �w�肳�ꂽ���W�̎l���Ƀ����_���ɕ\��
	mSpriteData.x = x - mSpriteData.width * mSpriteData.scale / 2.0f + (rand() % 2) * 20 - 10;
	mSpriteData.y = y - mSpriteData.height * mSpriteData.scale / 2.0f + (rand() % 2) * 20 - 10;
	mVisible = true;
	setCurrentFrame(mStartFrame);
}