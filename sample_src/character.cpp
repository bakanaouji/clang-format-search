//==========================================================
/// @file
/// @brief    character.h�̎���
/// @author   �������V
///
/// @attention  �v���C���[��G�N���X�̊�{�ƂȂ�N���X�B
///				�p�����邱�ƂŎg�p�\�ł��B
///				�p���Ȃ��ł̓I�u�W�F�N�g�̐����͂ł��܂���B

//==========================================================

#include "character.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
Character::Character() : Entity()
{
	// �����̕����͉E
	mDirection = characterNS::RIGHT;
	// Box�̏Փ˔�����g�p
	mCollisionType = entityNS::BOX;
	// �U������̃R���W�����͍ŏ��͂Ȃ�
	mAttackCollisionFlag = false;
	// ��Ԃ͉������Ȃ���Ԃ���X�^�[�g
	mState = characterNS::STATE::MOVE;
}

//==========================================================
// �p�����[�^������
//==========================================================
void Character::reset()
{
	// �U���A�j���[�V�����I����ɖ߂�ׂ��A�j���[�V�����t���[���ۑ��p
	mOldStartFrame = mStartFrame;
	mOldEndFrame = mEndFrame;
	// �����̕����͉E
	mDirection = characterNS::RIGHT;
	// �U������̃R���W�����͂Ȃ��Ń��Z�b�g
	mAttackCollisionFlag = false;
	// ��Ԃ͉������Ȃ���Ԃ���X�^�[�g
	mState = characterNS::STATE::MOVE;
	// �G���e�B�e�B�����Z�b�g
	Entity::reset();
}

//==========================================================
// �ړ����̃A�b�v�f�[�g�֐�
// �A�j���[�V�����ȊO���A�b�v�f�[�g
//==========================================================
void Character::updateMoving(const float frameTime)
{
	// �A�j���[�V�����̃A�b�v�f�[�g�͒P�Ƃōs����̂ł���ȊO���A�b�v�f�[�g
	Entity::updateWithoutImage(frameTime);
}

//==========================================================
// �U�����̃A�b�v�f�[�g�֐�
// ����ł͒ʏ�̃A�b�v�f�[�g
//==========================================================
void Character::updateAttacking(const float frameTime)
{
	Entity::update(frameTime);
}

//==========================================================
// ���S���ɌĂяo�����֐�
//==========================================================
void Character::dead()
{
	// �L�����N�^�[���A�N�e�B�u�ɂ���
	mActive = false;
	mVisible = false;
	mHealth = 0;
}

//==========================================================
// �ړ��\���`�F�b�N
//==========================================================
bool Character::checkCanMove(const float x, const float y)
{
	// 1�}�X32pixel�̂���32�Ŋ���
	// -16�͂߂荞�݂�h�~���邽�߂ɔ��}�X�������Ă�
	// +���͔������c
	int map_x = (int)((x - 16) / 32) + 1;
	int map_y = (int)((y - 16) / 32) + 1;
	if (map_x <= 0)
		map_x = 0;
	if (map_x >= mapNS::MAP_WIDTH)
		map_x = mapNS::MAP_WIDTH - 1;
	if (map_y <= 0)
		map_y = 0;
	if (map_y >= mapNS::MAP_HEIGHT)
		map_y = mapNS::MAP_HEIGHT - 1;

	// �i�����Ƃ���}�b�v�^�C���ɏ�Q�����������ꍇ�A
	if (mMap->getMapCol(map_y, map_x) == mapNS::COL_ROCK || mMap->getMapCol(map_y, map_x) == mapNS::COL_CASTLE)
	{
		// �ړ��s��
		return false;
	}
	else  // �i�����Ƃ���}�b�v�^�C���ɏ�Q�����Ȃ��ꍇ�A
	{
		// �ړ��\
		return true;
	}
}

//==========================================================
// �����Ă���������Z�b�g����
//==========================================================
void Character::setDirection(const characterNS::DIRECTION dir)
{
	// �Z�b�g����L�����N�^�[�̌����ɂ���ĕ���
	switch (dir)
	{
	case characterNS::LEFT:
		// �������������Ă��Ȃ���΍������ɃA�j���[�V���������Z�b�g
		if (mDirection != characterNS::DIRECTION::LEFT)
		{
			mDirection = characterNS::DIRECTION::LEFT;
			mStartFrame = mMoveLeftStartFrame;
			mCurrentFrame = mEndFrame - mCurrentFrame + mMoveLeftStartFrame;
			mEndFrame = mMoveLeftEndFrame;
			setRect();
		}
		break;
	case characterNS::RIGHT:
		// �E�����������Ă��Ȃ���ΉE�����ɃA�j���[�V���������Z�b�g
		if (mDirection != characterNS::DIRECTION::RIGHT)
		{
			mDirection = characterNS::DIRECTION::RIGHT;
			mStartFrame = mMoveRightStartFrame;
			mCurrentFrame = mEndFrame - mCurrentFrame + mMoveRightStartFrame;
			mEndFrame = mMoveRightEndFrame;
			setRect();
		}
		break;
	case characterNS::UP:
		// ������������Ă��Ȃ���Ώ�����ɃA�j���[�V���������Z�b�g
		if (mDirection != characterNS::DIRECTION::UP)
		{
			mDirection = characterNS::DIRECTION::UP;
			mStartFrame = mMoveUpStartFrame;
			mCurrentFrame = mEndFrame - mCurrentFrame + mMoveUpStartFrame;
			mEndFrame = mMoveUpEndFrame;
			setRect();
		}
		break;
	case characterNS::DOWN:
		// �������������Ă��Ȃ���Ή������ɃA�j���[�V���������Z�b�g
		if (mDirection != characterNS::DIRECTION::DOWN)
		{
			mDirection = characterNS::DIRECTION::DOWN;
			mStartFrame = mMoveDownStartFrame;
			mCurrentFrame = mEndFrame - mCurrentFrame + mMoveDownStartFrame;
			mEndFrame = mMoveDownEndFrame;
			setRect();
		}
		break;
	default:
		break;
	}
}

//==========================================================
// �ړ����ւƏ�Ԃ�J��
//==========================================================
void Character::changeStateToMove()
{
	// ��Ԃ͈ړ����ւƑJ��
	mState = characterNS::MOVE;
	// �A�j���[�V�����t���[���̑J�ڂ͐�������
	mMode = imageNS::HORIZONTAL;
	// �ړ����̓A�j���[�V�����̓��[�v������
	mLoop = true;
	// �A�j���[�V�����������Ă�������ɍ��킹�ăZ�b�g
	mStartFrame = mOldStartFrame;
	mEndFrame = mOldEndFrame;
	setCurrentFrame(mStartFrame);
}