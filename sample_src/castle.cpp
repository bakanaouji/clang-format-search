//==========================================================
/// @file
/// @brief    castle.h�̎���
/// @author   �������V
///
/// @attention  ���\���N���X�ł��B
///				�v���C���[�Ɠ��l�ɁA��̗̑͂�0�ɂȂ�ƃQ�[���[�I�[�o�[�ł��B

//==========================================================

#include "castle.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
Castle::Castle() : Entity()
{
	mSpriteData.width = castleNS::WIDTH;			// �P�̉摜�̃T�C�Y
	mSpriteData.height = castleNS::HEIGHT;
	mSpriteData.rect.bottom = castleNS::HEIGHT;	// �摜���̑I�����镔��
	mSpriteData.rect.right = castleNS::WIDTH;
	mSpriteData.x = castleNS::X;					// ��ʏ�̈ʒu
	mSpriteData.y = castleNS::Y;
	mCols = castleNS::TEXTURE_COLS;				// �e�N�X�`���̗�
	mStartFrame = castleNS::START_FRAME;			// �A�j���[�V�����̍ŏ��̃t���[��
	mCurrentFrame = mStartFrame;					// ���݂̃t���[�����A�j���[�V�����̍ŏ��̃t���[���ɃZ�b�g
	// Box�̏Փ˔���p
	mEdge.left = (LONG)(-castleNS::WIDTH / 1.8);
	mEdge.right = (LONG)(castleNS::WIDTH / 1.8);
	mEdge.top = (LONG)(-castleNS::HEIGHT / 1.8);
	mEdge.bottom = (LONG)(castleNS::HEIGHT / 1.8);
	mCollisionType = entityNS::BOX;

	mDeath = false;								// ���S���Ă��Ȃ���Ԃ���X�^�[�g
}

//==========================================================
// �p�����[�^������
//==========================================================
void Castle::reset()
{
	mDeath = false;								// ���S���Ă��Ȃ���ԂɃ��Z�b�g
	// �G���e�B�e�B�����Z�b�g
	Entity::reset();
}

//==========================================================
// Update
// �A�j���[�V�����̍X�V���s���B
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����𐧌䂷�邽�߂Ɏg�p
//==========================================================
void Castle::update(const float frameTime)
{
	// ��A�N�e�B�u�Ȃ�A�������Ȃ�
	if (mVisible == false)
		return;


	// �_���[�W���󂯂Ă���Ȃ��莞�Ԃ��ƂɃA�j���[�V������_��
	if (mIsDamaged)
	{
		// �_���[�W���ɃA�j���[�V�����p�̃^�C�}�[�����Z
		mDamageTimer += frameTime;
		mTotalDamageTime += frameTime;
		// 0.15�b���Ƃɕ`��t���O��؂�ւ�
		if (mDamageTimer > 0.15f)
		{
			if (mDrawFlag)
				mDrawFlag = false;
			else
				mDrawFlag = true;
			mDamageTimer = 0.0f;
		}
		// DAMAGE_TIME�������_�ł�����A�_�ł��I��
		if (mTotalDamageTime > 1.0f)
		{
			mDamageTimer = 0.0f;
			mTotalDamageTime = 0.0f;
			mDrawFlag = true;
			mIsDamaged = false;
		}
	}
	// �G���e�B�e�B���X�V
	Entity::update(frameTime);
}

//==========================================================
// �_���[�W����
// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
//==========================================================
void Castle::damage(const WEAPON weapon)
{
	// ��A�N�e�B�u�ȏꍇ�A�_���[�W�͎󂯂Ȃ�
	if (!mActive)
		return;

	// WEAPON�ɂ���ă_���[�W�𕪊�
	switch (weapon)
	{
	case FIRE:					// ��
		break;
	case BRAVE_ATTACK:			// �E�҂̍U��
		break;
	case BRAVE_SECOND_ATTACK:	// �E�҂̑��
		break;
	case ENEMY_ATTACK:			// �G���G�̍U��
		// ���_���[�W���󂯂�
		mHealth -= ENEMY_ATTACK_DAMAGE * mDamagePer;
		// �_���[�W��Ԃ̃t���O���I��
		mIsDamaged = true;
		break;
	case MIDBOSS_ATTACK:		// ���{�X�̍U��
		// ���_���[�W���󂯂�
		mHealth -= MIDBOSS_ATTACK_DAMAGE * mDamagePer;
		// �_���[�W��Ԃ̃t���O���I��
		mIsDamaged = true;
		break;
	default:
		break;
	}
	// �̗͂�0�ȉ��ɂȂ����Ȃ�A���S
	if (mHealth <= 0)
		mDeath = true;
}