//==========================================================
/// @file
/// @brief    attackEffect.h�̎���
/// @author   �������V
///
/// @attention  �U�����̃A�j���[�V������\������N���X�ł��B

//==========================================================

#include "attackEffect.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
AttackEffect::AttackEffect() : Image()
{
	mVisible = false;								// ��\�����f�t�H���g
	mSpriteData.width = attackEffectNS::WIDTH;		// �P�̉摜�̃T�C�Y
	mSpriteData.height = attackEffectNS::HEIGHT;
	mSpriteData.rect.bottom = attackEffectNS::HEIGHT;// �摜���̑I�����镔��
	mSpriteData.rect.right = attackEffectNS::WIDTH;
	mCols = attackEffectNS::TEXTURE_COLS;			// �e�N�X�`���̗�
	mFrameDelay = attackEffectNS::ANIMATION_DELAY;	// �A�j���[�V�����̊e�t���[���̊Ԋu
	mStartFrame = attackEffectNS::START_FRAME;		// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = attackEffectNS::END_FRAME;			// �A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;						// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
	mSpriteData.scale = attackEffectNS::SCALE;		// �X�v���C�g�̃X�P�[��
	mLoop = false;									// �A�j���[�V�����̓��[�v�����Ȃ�
}

//==========================================================
// Update
// �A�j���[�V�������X�V����B
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//==========================================================
void AttackEffect::update (const float frameTime)
{
	// �X�v���C�g����\���Ȃ牽�����Ȃ�
	if (getVisible() == false)
		return;
	// �摜���X�V
	Image::update(frameTime);
	// �A�j���[�V�������I�������ꍇ�A�X�v���C�g���\���ɂ���
	if (getAnimationComplete())
		setVisible(false);
}

//==========================================================
// Attack
// �U���̏Ռ��g�̃A�j���[�V�������w�肳�ꂽ�ʒu�ɔ���
//==========================================================
void AttackEffect::attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction)
{
	// brave�i�v���C���[�j�̌����ɂ���ĕ\��������ʒu�𕪊�
	// �v���C���[�̌��X�̉摜���΂��Ă���̂ŁA�}10���x�̕␳�������Ă���
	switch (direction)
	{
	case characterNS::RIGHT:	// �E
		setX(centerX - getWidth() * getScale() / 2.0f);
		setY(centerY - getHeight() * getScale() / 2.0f - 10);
		setDegrees(0);
		flipHorizontal(false);
		break;
	case characterNS::LEFT:		// ��
		setX(centerX - getWidth() * getScale() / 2.0f - 10);
		setY(centerY - getHeight() * getScale() / 2.0f - 10);
		setDegrees(0);
		flipHorizontal(true);
		break;
	case characterNS::UP:		// ��
		setX(centerX - getWidth() * getScale() / 2.0f - 10);
		setY(centerY - getHeight() * getScale() / 2.0f - 10 - 10);
		setDegrees(40.0f);
		flipHorizontal(true);
		break;
	case characterNS::DOWN:		// ��
		setX(centerX - getWidth() * getScale() / 2.0f - 10);
		setY(centerY - getHeight() * getScale() / 2.0f - 10 + 10);
		setDegrees(230.0f);
		flipHorizontal(true);
		break;
	}
	// �X�v���C�g��\��
	mVisible = true;
	// ���݂̃t���[�����A�j���[�V�����̍ŏ��̃t���[���ɃZ�b�g
	setCurrentFrame(mStartFrame);
}