//==========================================================
/// @file
/// @brief    fire.h�̎���
/// @author   �������V
///
/// @attention  �v���C���[�̕K�E�Z�ɂ���Ĕ��˂���鉊��\���N���X�ł��B

//==========================================================

#include "fire.h"

//==========================================================

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
Fire::Fire() : Entity()
{
	mActive = false;								// ���̓A�N�e�B�u�łȂ���Ԃ���J�n
	mVisible = false;
	mSpriteData.width = fireNS::WIDTH;			// �P�̉摜�̃T�C�Y
	mSpriteData.height = fireNS::HEIGHT;
	mSpriteData.rect.bottom = fireNS::HEIGHT;	// �摜���̑I�����镔��
	mSpriteData.rect.right = fireNS::WIDTH;
	mCols = fireNS::TEXTURE_COLS;				// �e�N�X�`���̗�
	mFrameDelay = fireNS::ANIMATION_DELAY;		// �A�j���[�V�����̊e�t���[���̊Ԋu
	mStartFrame = fireNS::START_FRAME;			// �t�@�C�A�[�A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = fireNS::END_FRAME;				// �t�@�C�A�[�A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;					// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
	mCollisionType = entityNS::CIRCLE;			// �~�̏Փ˔���
	mRadius = fireNS::COLLISION_RADIUS;			// �~�̏Փ˔���p
	mFireTimer = 0.0f;							// �������ˉ\�ɂȂ�܂ł̎c�莞��
	mMode = imageNS::VERTICAL;					// �A�j���[�V�����̃��[�h
}

//==========================================================
// �p�����[�^������
//==========================================================
void Fire::reset()
{
	mActive = false;								// ���̓A�N�e�B�u�łȂ���Ԃ���J�n	
	mVisible = false;
	mFrameDelay = fireNS::ANIMATION_DELAY;		// �A�j���[�V�����̊e�t���[���̊Ԋu
	mStartFrame = fireNS::START_FRAME;			// �t�@�C�A�[�A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = fireNS::END_FRAME;				// �t�@�C�A�[�A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;					// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
	mCollisionType = entityNS::CIRCLE;			// �~�̏Փ˔���
	mRadius = fireNS::COLLISION_RADIUS;			// �~�̏Փ˔���p
	mFireTimer = 0.0f;							// �������ˉ\�ɂȂ�܂ł̎c�莞��
	mMode = imageNS::VERTICAL;					// �A�j���[�V�����̃��[�h
}

//==========================================================
// Update
// �G���e�B�e�B���A�N�e�B�u�ȏꍇ�A��莞�Ԃ܂������ړ�
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
// �����FframeTime�@1�t���[���Ōo�߂�������
//==========================================================
void Fire::update(const float frameTime)
{
	mFireTimer -= frameTime;						// ���˂��\�ɂȂ�܂ł̎c�莞��
	
	if (mVisible == false)
		return;

	if (mFireTimer < 0)							// ���ˉ\�ȏꍇ
	{
		mVisible = false;						// �Â����𖳌���
		mActive = false;
	}

	Image::update(frameTime);

	mSpriteData.x += frameTime * mVelocity.x;		// X�����Ɉړ�
	mSpriteData.y += frameTime * mVelocity.y;		// Y�����Ɉړ�
	// ��ʂ̒[�܂ŗ��������
	if (mSpriteData.x > GAME_WIDTH || mSpriteData.x < 0 || mSpriteData.y < 0 || mSpriteData.y > GAME_HEIGHT)
	{
		mVisible = false;
		mActive = false;
	}
}

//==========================================================
// Fire
// Brave���牊�𔭎�
//==========================================================
void Fire::fire(const float centerX, const float centerY, const float width, const float height, const float scale, int& mp, const characterNS::DIRECTION direction)
{
	if (mFireTimer <= 0.0f && mp > fireNS::FIRE_MP)	// ���ˉ\�ȏꍇ
	{
		mAudio->playCue("fire");
		switch (direction)
		{
		case characterNS::UP:
			mVelocity.x = 0.0f;
			mVelocity.y = -fireNS::SPEED;
			mSpriteData.angle = (float)PI / 2.0f;
			break;
		case characterNS::RIGHT:
			mVelocity.x = fireNS::SPEED;
			mVelocity.y = 0.0f;
			mSpriteData.angle = (float)-PI;
			break;
		case characterNS::DOWN:
			mVelocity.x = 0.0f;
			mVelocity.y = fireNS::SPEED;
			mSpriteData.angle = (float)-PI / 2;
			break;
		case characterNS::LEFT:
			mVelocity.x = -fireNS::SPEED;
			mVelocity.y = 0.0f;
			mSpriteData.angle = 0.0f;
			break;
		}
		mSpriteData.x = (float)(centerX - mSpriteData.width / 2 - width * (scale - 1) / 2.0);
		mSpriteData.y = (float)(centerY - mSpriteData.height / 2 - height * (scale - 1) / 2.0f - 10.0f);
		mVisible = true;									// ����\��
		mActive = true;									// �Փˉ\�ɂ���
		mFireTimer = fireNS::FIRE_DELAY;					// ���˂̊Ԋu
		mp -= fireNS::FIRE_MP;							// MP������
	}
}