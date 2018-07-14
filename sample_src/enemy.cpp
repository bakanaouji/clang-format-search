//==========================================================
/// @file
/// @brief    enemy.h�̎���
/// @author   �������V
///
/// @attention  �G���G��\���N���X�ł��B
///				�o���P�[�h��j�󂵂Ȃ����Ɍ������čU�߂Ă���̂ŁA
///				�v���C���[�͗E�҂𑀍삵�ē|���܂��B

//==========================================================

#include "enemy.h"
#include <iostream>

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
Enemy::Enemy() : Character()
{
	mSpriteData.width = enemyNS::WIDTH;								// �G���G�̃T�C�Y
	mSpriteData.height = enemyNS::HEIGHT;
	mSpriteData.rect.bottom = enemyNS::HEIGHT;						// ��ʂ̈ꕔ��I��
	mSpriteData.rect.right = enemyNS::WIDTH;
	mFrameDelay = enemyNS::MOVE_ANIMATION_DELAY;						// �A�j���[�V�����̊e�t���[���̊Ԋu
																	// �����̕����͉E
	mGoalDirection = characterNS::RIGHT;								// ���ɐi�ނׂ�����
	mOldDirection = mDirection;										// �U�����O�Ɍ����Ă�������
	mStartFrame = enemyNS::MOVE_RIGHT_START_FRAME;					// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = enemyNS::MOVE_RIGHT_END_FRAME;						// �A�j���[�V�����̍Ō�̃t���[��
	mOldStartFrame = mStartFrame;										// �U���A�j���[�V�����I����ɖ߂�ׂ��A�j���[�V�����̍ŏ��̃t���[��
	mOldEndFrame = mEndFrame;											// �U���A�j���[�V�����I����ɖ߂�ׂ��A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;										// ���݂̃t���[�����A�j���[�V�����̍ŏ��̃t���[���ɃZ�b�g
	// Box�̏Փ˔���p
	mEdge.left = (LONG)(-enemyNS::WIDTH * mSpriteData.scale / 2.0);
	mEdge.right = (LONG)(enemyNS::WIDTH * mSpriteData.scale / 2.0);
	mEdge.top = (LONG)(-enemyNS::HEIGHT * mSpriteData.scale / 2.0);
	mEdge.bottom = (LONG)(enemyNS::HEIGHT * mSpriteData.scale / 2.0);

	mCanMakeDecesionMove = false;									// �ړ��Ɋւ��Ă̈ӎv������s���邩�ǂ���
	mAttackAnimationMode = imageNS::VERTICAL;						// �U�����̃A�j���[�V�����̑J�ڃ��[�h�i�����A�����j
	mMoveUpStartFrame = enemyNS::MOVE_UP_START_FRAME;				// ������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveUpEndFrame = enemyNS::MOVE_UP_END_FRAME;					// ������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveRightStartFrame = enemyNS::MOVE_RIGHT_START_FRAME;			// �E�����ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveRightEndFrame = enemyNS::MOVE_RIGHT_END_FRAME;				// �E�����ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveDownStartFrame = enemyNS::MOVE_DOWN_START_FRAME;			// �������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveDownEndFrame = enemyNS::MOVE_DOWN_END_FRAME;				// �������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveLeftStartFrame = enemyNS::MOVE_LEFT_START_FRAME;			// �������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveLeftEndFrame = enemyNS::MOVE_LEFT_END_FRAME;				// �������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mAttackUpStartFrame = enemyNS::ATTACK_UP_START_FRAME;			// ������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackUpEndFrame = enemyNS::ATTACK_UP_END_FRAME;				// ������U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackRightStartFrame = enemyNS::ATTACK_RIGHT_START_FRAME;		// �E�����U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackRightEndFrame = enemyNS::ATTACK_RIGHT_END_FRAME;			// �E�����U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackDownStartFrame = enemyNS::ATTACK_DOWN_START_FRAME;		// �������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackDownEndFrame = enemyNS::ATTACK_DOWN_END_FRAME;			// �������U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackLeftStartFrame = enemyNS::ATTACK_LEFT_START_FRAME;		// �������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackLeftEndFrame = enemyNS::ATTACK_LEFT_END_FRAME;			// �������U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackTime = enemyNS::ATTACK_TIME;								// �v���C���[�ɗאڂ��Ă���U���Ɉڂ�܂ł̗��ߎ���
	mContinueAttackTime = enemyNS::CONTINUE_ATTACK_TIME;				// �A���ōU������ꍇ�̗��ߎ���
	mDamagePer = 1.0f;
}

//==========================================================
// �p�����[�^������
//==========================================================
void Enemy::reset()
{
	Character::reset();
	// �����̕����͉E
	mOldDirection = mDirection;
	mGoalDirection = characterNS::RIGHT;
	// �ړ��Ɋւ��Ă̈ӎv����͍s���Ȃ�
	mCanMakeDecesionMove = false;
}

//=============================================================================
// Update
// �U����������̏�ԑJ�ڂ�ړ��Ȃǂ̍s�����s���B
// �S�̓I�ȏ�ԑJ�ڂɂ��ẮA�Փ˔���ɂ���čs������stage.collisions()�ɋL��
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Enemy::update(const float frameTime)
{
	// ��A�N�e�B�u�Ȃ牽�����Ȃ�
	if (!mActive)
		return;
	
	// �U�����肪�łĂ���ꍇ�̓R���W�����𐶐����ē����蔻����Ƃ�
	if (mAttackCollisionFlag) {
		mAttackCollision.attack(getCenterX(), getCenterY(), (float)getWidth(), (float)getHeight(), mDirection);
	}
	// �U�����̃G���e�B�e�B���o���t���O���I�t
	mAttackCollisionFlag = false;
	// �ړ��Ɋւ��Ă̈ӎv����̓f�t�H���g�ł͍s���Ȃ�
	mCanMakeDecesionMove = false;

	// �����ŏ�ԑJ�ځA���W�̈ړ��A�A�j���[�V�����̃��Z�b�g���s��
	// ���ꂼ��̏�Ԃ��Ƃɏ����𕪊�
	switch (mState)
	{
	case characterNS::MOVE:	// �ړ����Ȃ�ڕW�̕����ɂ܂������i��
		// �ڕW�̕����Ɍ������Đi��
		switch (mGoalDirection)
		{
		case characterNS::LEFT:
			if (mDirection != mGoalDirection)
				changeDirection(mMoveLeftStartFrame, mMoveLeftEndFrame);
			// ���Ɉړ�
			mSpriteData.x -= ENEMY_MOVE_SPEED * frameTime;
			break;
		case characterNS::RIGHT:
			if (mDirection != mGoalDirection)
				changeDirection(mMoveRightStartFrame, mMoveRightEndFrame);
			// �E�Ɉړ�
			mSpriteData.x += ENEMY_MOVE_SPEED * frameTime;
			break;
		case characterNS::UP:
			if (mDirection != mGoalDirection)
				changeDirection(mMoveUpStartFrame, mMoveUpEndFrame);
			// ��Ɉړ�
			mSpriteData.y -= ENEMY_MOVE_SPEED * frameTime;
			break;
		case characterNS::DOWN:
			if (mDirection != mGoalDirection)
				changeDirection(mMoveDownStartFrame, mMoveDownEndFrame);
			// ���Ɉړ�
			mSpriteData.y += ENEMY_MOVE_SPEED * frameTime;
			break;
		}
		// �U������ł͂Ȃ��͂��Ȃ̂Ńt���O���I�t
		mIsAttacked = false;
		// �摜�̂݃A�b�v�f�[�g
		Entity::updateOnlyImage(frameTime);
		if (isCenterOfTile())	// �^�C���̒����ɗ�����}�b�v���X�V
		{
			// �}�b�v�Ɏ����̈ʒu��ǉ�
			mMap->updateMapCol((float)(mTileY * mapNS::TEXTURE_SIZE), (float)(mTileX * mapNS::TEXTURE_SIZE), mMap->getMapCol(mTileY, mTileX) + mapNS::COL_ENEMY_INCLEMENT);
			// �ړ��Ɋւ��Ă̈ӎv���肪�\
			mCanMakeDecesionMove = true;
		}
		break;
	case characterNS::ATTACK:	// �U����
		// �A�j���[�V�������I�����Ă�����U���I��
		// ��Ԃ��ړ����ɖ߂�
		if (mAnimComplete)
		{
			// �ړ��Ɋւ��Ă̈ӎv���肪�\
			mCanMakeDecesionMove = true;
			// �U������Ȃ̂Ńt���O���I��
			mIsAttacked = true;
			// ��Ԃ��ړ����ւƑJ��
			changeStateToMove();
			// �����Ă��������߂�
			mDirection = mOldDirection;
			Entity::updateOnlyImage(frameTime);
		}
		break;
	case characterNS::PRE_ATTACK:	// �U�����O
		// �^�C�}�[�����Z�A��莞�Ԃ𒴂�����U���Ɉڂ�
		mAttackTimer += frameTime;
		// �U������Ȃ�cuntinueAttackTime��ɍU��
		if (mIsAttacked && mAttackTimer > mContinueAttackTime)
		{
			mAttackTimer = 0.0f;
			mState = characterNS::ATTACK;
			mAttackCollisionFlag = true;
		}
		// �U������łȂ��Ȃ�attackTime��ɍU��
		else if (!mIsAttacked && mAttackTimer > mAttackTime)
		{
			mAttackTimer = 0.0f;
			mState = characterNS::ATTACK;
			mAttackCollisionFlag = true;
		}
		break;
	case characterNS::WAIT:		// �ҋ@��
		// �ړ��Ɋւ��Ă̈ӎv���肪�\
		mCanMakeDecesionMove = true;
		break;
	case characterNS::DEATH:	// ���S��
		// �`��͉\
		mDrawFlag = true;
		// �_���[�W�͖���
		mIsDamaged = false;
		// �W�����v���Ȃ̂ŁA�d�͂ɏ]���Č���������
		mVelocity.y += frameTime * 2000.0f;
		// xy�����ɃW�����v
		mSpriteData.x += frameTime * mVelocity.x;
		mSpriteData.y += frameTime * mVelocity.y;
		// �Q�[����ʂ�艺�ɏo����A�G���e�B�e�B���A�N�e�B�u��
		if (mSpriteData.y > GAME_HEIGHT)
			dead();
	default:
		break;
	}

	// ��ԑJ�ڌ�̏���
	// �ړ����̓L�[��������Ă���Ԃ̂݃A�j���[�V�������i�ނ悤�ɂ������̂ŁA
	// �G���e�B�e�B�ƃA�j���[�V�����̍X�V��Ɨ��ɍs���Ă���
	switch (mState)
	{
	case characterNS::MOVE:
		updateMoving(frameTime);
		break;
	case characterNS::ATTACK:
		updateAttacking(frameTime);
		break;
	case characterNS::PRE_ATTACK:
		Entity::updateWithoutImage(frameTime);
		break;
	case characterNS::WAIT:
		Entity::updateWithoutImage(frameTime);
	default:
		Entity::update(frameTime);
		break;
	}

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
		if (mTotalDamageTime > enemyNS::DAMAGE_TIME)
		{
			mDamageTimer = 0.0f;
			mTotalDamageTime = 0.0f;
			mDrawFlag = true;
			mIsDamaged = false;
		}
	}
	// �U���p�G���e�B�e�B���X�V
	mAttackCollision.update(frameTime);
}

//==========================================================
// �_���[�W����
// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
//==========================================================
void Enemy::damage(const WEAPON weapon)
{
	// �G�̃^�C�v�ɂ���āA�󂯂�_���[�W�̊�����ω�
	if (mType == enemyNS::NORMAL)
		setDamagePer(1.0f);
	else if (mType == enemyNS::RED)
		setDamagePer(0.8f);
	else if (mType == enemyNS::BLUE)
		setDamagePer(0.9f);

	// WEAPON�ɂ���ă_���[�W�𕪊�
	switch (weapon)
	{
	case FIRE:					// ��
		// ���_���[�W���󂯂�
		mHealth -= BRAVE_FIRE_DAMAGE * mDamagePer;
		// �_���[�W��Ԃ̃t���O���I��
		mIsDamaged = true;
		break;
	case BRAVE_ATTACK:			// �E�҂̍U��
		// ���_���[�W���󂯂�
		mHealth -= BRAVE_ATTACK_DAMAGE * mDamagePer;
		// �_���[�W��Ԃ̃t���O���I��
		mIsDamaged = true;
		break;
	case BRAVE_SECOND_ATTACK:	// �E�҂̑��
		// ���_���[�W���󂯂�
		mHealth -= BRAVE_ATTACK_DAMAGE * mDamagePer;
		// �_���[�W��Ԃ̃t���O���I��
		mIsDamaged = true;
		break;
	case ENEMY_ATTACK:
		break;
	default:
		break;
	}
	// �̗͂�0�ȉ��ɂȂ�����A
	if (mHealth <= 0)
	{
		mIsDamaged = false;
		// ��Ԃ����S���ɑJ��
		mState = characterNS::DEATH;
		// �K���ȕ����փW�����v���Ȃ����ʊO�ւƗ����Ă���
		if (rand() % 2 == 0)
			mVelocity.x = 32.0f * 2.0;
		else
			mVelocity.x = -32.0f * 2.0;
		mVelocity.y = -sqrt(2 * 2000.0f * 96 * 2);
		mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
	}
}

//==========================================================
// �����Ă��������ύX���郁�\�b�h
//==========================================================
void Enemy::changeDirection(const int strF, const int endF)
{
	mDirection = mGoalDirection;
	mStartFrame = strF;
	mEndFrame = endF;
	setCurrentFrame(mStartFrame);
}

//==========================================================
// ��ԋ߂��̃o���P�[�h�̃C���f�b�N�X��Ԃ��֐�
//==========================================================
int Enemy::searchNearBarricadeIndex()
{
	// �ŏ��̋�����
	int minX = 0, minY = 0;
	// �ŏ��̋����̃C���f�b�N�X
	int minIndex = 0;
	// �A�N�e�B�u�ȃo���P�[�h�̐��𐔂���p
	int count = 0;

	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		// �����o���P�[�h���A�N�e�B�u�Ȃ�
		if (mBarricades[i].getActive())
		{
			// �T��1�ڂ̏ꍇ
			if (count == 0)
			{
				minX = (int)abs(mBarricades[i].getTileX() - getTileX());
				minY = (int)abs(mBarricades[i].getTileY() - getTileY());
				minIndex = i;
			}
			else // 2�ڈȍ~
			{
				if ((mBarricades[i].getTileX() - getTileX())*(mBarricades[i].getTileX() - getTileX()) +
					(mBarricades[i].getTileY() - getTileY())*(mBarricades[i].getTileY() - getTileY()) <
					minX * minX + minY * minY)
				{
					// �V���ɑ��
					minX = (int)abs(mBarricades[i].getTileX() - getTileX());
					minY = (int)abs(mBarricades[i].getTileY() - getTileY());
					minIndex = i;
				}
			}
			// �J�E���g�𑝂₷
			count++;
		}
	}
	mNearBarricadeIndex =  minIndex;
	return minIndex;
}

//==========================================================
// �G���G�����܂ł̒�����Ƀo���P�[�h�����݂�����true�A���݂��Ȃ��ꍇ��false��Ԃ��B
//==========================================================
bool Enemy::checkBarricadeOnLine() {
	// �G���G�̏㉺���E���ꂼ��ɑ΂��āA�G���G����߂����ɏ邪�Ȃ������`�F�b�N
	// ���ꂼ��̕����ɂ��āA��ȊO�̃I�u�W�F�N�g����Ɍ��������ꍇ��true��Ԃ�
	// �����ꂩ�̕����ɂ��āA��܂ł̒�����ɂȂ�̃I�u�W�F�N�g���Ȃ��ꍇ��false��Ԃ�
	for (int x = mTileX; x <= mapNS::MAP_WIDTH; ++x)
	{
		if (mMap->getMapObj(mTileY, x) == mapNS::OBJ_BARRICADE || mMap->getMapCol(mTileY, x) == mapNS::COL_ROCK)
			break;
		if (mMap->getMapCol(mTileY, x) == mapNS::COL_CASTLE)
		{
			mGoalDirection = characterNS::RIGHT;
			return false;
		}
	}
	for (int x = mTileX; x >= 0; --x)
	{
		if (mMap->getMapObj(mTileY, x) == mapNS::OBJ_BARRICADE || mMap->getMapCol(mTileY, x) == mapNS::COL_ROCK)
			break;
		if (mMap->getMapCol(mTileY, x) == mapNS::COL_CASTLE)
		{
			mGoalDirection = characterNS::LEFT;
			return false;
		}
	}
	for (int y = mTileY; y <= mapNS::MAP_HEIGHT; ++y)
	{
		if (mMap->getMapObj(y, mTileX) == mapNS::OBJ_BARRICADE || mMap->getMapCol(y, mTileX) == mapNS::COL_ROCK)
			break;
		if (mMap->getMapCol(y, mTileX) == mapNS::COL_CASTLE)
		{
			mGoalDirection = characterNS::DOWN;
			return false;
		}
	}
	for (int y = mTileY; y >= 0; --y)
	{
		if (mMap->getMapObj(y, mTileX) == mapNS::OBJ_BARRICADE || mMap->getMapCol(y, mTileX) == mapNS::COL_ROCK)
			break;
		if (mMap->getMapCol(y, mTileX) == mapNS::COL_CASTLE)
		{
			mGoalDirection = characterNS::UP;
			return false;
		}
	}
	return true;
}

//==========================================================
// �U�����[�h�Ƀ`�F���W����Ƃ��ɌĂяo���֐�
//==========================================================
void Enemy::changeAttack(VECTOR2 &collisionVector)
{
	// �U�����O���[�h�Ƀ`�F���W
	mLoop = false;
	// �Փ˃x�N�g���𗘗p���āA�ՓˑΏۂ���������������悤�ɐݒ�
	float yPerx = (float)collisionVector.y / collisionVector.x;
	float hPerw = (float)mSpriteData.height / mSpriteData.width;
	if (collisionVector.x > 0)
	{
		if (yPerx > -hPerw && yPerx < hPerw)
		{
			mGoalDirection = characterNS::RIGHT;
			changeDirection(mMoveRightStartFrame, mMoveRightEndFrame);
		}
		else if (yPerx < -hPerw)
		{
			mGoalDirection = characterNS::UP;
			changeDirection(mMoveUpStartFrame, mMoveUpEndFrame);
		}
		else
		{
			mGoalDirection = characterNS::DOWN;
			changeDirection(mMoveDownStartFrame, mMoveDownEndFrame);
		}
	}
	else
	{
		if (yPerx > -hPerw && yPerx < hPerw)
		{
			mGoalDirection = characterNS::LEFT;
			changeDirection(mMoveLeftStartFrame, mMoveLeftEndFrame);
		}
		else if (yPerx < -hPerw)
		{
			mGoalDirection = characterNS::DOWN;
			changeDirection(mMoveDownStartFrame, mMoveDownEndFrame);
		}
		else
		{
			mGoalDirection = characterNS::UP;
			changeDirection(mMoveUpStartFrame, mMoveUpEndFrame);
		}
	}
	// ���ɐi�ނׂ������̓��Z�b�g
	mGoalDirection = characterNS::NONE;
	// �U�����O�ɏ�Ԃ�J��
	mState = characterNS::PRE_ATTACK;
	// �A�j���[�V�����̃��[�h��ݒ�
	mMode = mAttackAnimationMode;
	// �A�j���[�V�����I�����Ƀt���[����߂����߂ɕۑ�
	mOldStartFrame = mStartFrame;
	mOldEndFrame = mEndFrame;
	mOldDirection = mDirection;

	// �����ɉ����ăA�j���[�V������؂�ւ�
	switch (mDirection)
	{
	case characterNS::LEFT:
		mStartFrame = mAttackLeftStartFrame;
		mEndFrame = mAttackLeftEndFrame;
		break;
	case characterNS::RIGHT:
		mStartFrame = mAttackRightStartFrame;
		mEndFrame = mAttackRightEndFrame;
		break;
	case characterNS::UP:
		mStartFrame = mAttackUpStartFrame;
		mEndFrame = mAttackUpEndFrame;
		break;
	case characterNS::DOWN:
		mStartFrame = mAttackDownStartFrame;
		mEndFrame = mAttackDownEndFrame;
		break;
	default:
		break;
	}
	setCurrentFrame(mStartFrame);
	return;
}

//==========================================================
// �U�����[�h�Ƀ`�F���W����Ƃ��ɌĂяo���֐�
//==========================================================
void Enemy::changeAttack(const characterNS::DIRECTION dir)
{
	// �U�����O���[�h�Ƀ`�F���W
	mLoop = false;
	// ������ύX
	mGoalDirection = dir;
	switch (dir)
	{
	case characterNS::LEFT:
		changeDirection(mMoveLeftStartFrame, mMoveLeftEndFrame);
		break;
	case characterNS::RIGHT:
		changeDirection(mMoveRightStartFrame, mMoveRightEndFrame);
		break;
	case characterNS::UP:
		changeDirection(mMoveUpStartFrame, mMoveUpEndFrame);
		break;
	case characterNS::DOWN:
		changeDirection(mMoveDownStartFrame, mMoveDownEndFrame);
		break;
	case characterNS::NONE:
		break;
	default:
		break;
	}
	// ���ɐi�ނׂ������̓��Z�b�g
	mGoalDirection = characterNS::NONE;
	// �U�����O�ɏ�Ԃ�J��
	mState = characterNS::PRE_ATTACK;
	// �A�j���[�V�����̃��[�h��ݒ�
	mMode = mAttackAnimationMode;
	// �A�j���[�V�����I�����Ƀt���[����߂����߂ɕۑ�
	mOldStartFrame = mStartFrame;
	mOldEndFrame = mEndFrame;
	mOldDirection = mDirection;
	// �����ɉ����ăA�j���[�V������؂�ւ�
	switch (mDirection)
	{
	case characterNS::LEFT:
		mStartFrame = mAttackLeftStartFrame;
		mEndFrame = mAttackLeftEndFrame;
		break;
	case characterNS::RIGHT:
		mStartFrame = mAttackRightStartFrame;
		mEndFrame = mAttackRightEndFrame;
		break;
	case characterNS::UP:
		mStartFrame = mAttackUpStartFrame;
		mEndFrame = mAttackUpEndFrame;
		break;
	case characterNS::DOWN:
		mStartFrame = mAttackDownStartFrame;
		mEndFrame = mAttackDownEndFrame;
		break;
	default:
		break;
	}
	setCurrentFrame(mStartFrame);
	return;
}

//==========================================================
// �^�C���̒����ɃL�����N�^�[�����邩�ǂ���
//==========================================================
bool Enemy::isCenterOfTile()
{
	bool isCenter = false;
	// �㉺���E�ǂ��Ɉړ����Ă��邩�ɂ���Ē����ɂ��邩�̔���𕪊�
	// ��{�I�ɂ́A������^�C�����ׂ�����^�C�����ړ����Ē����ɗ����Ɣ���
	switch (mGoalDirection)
	{
	case characterNS::RIGHT:
		if (mSpriteData.x / 32 >= mTileX + 1)
		{
			mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
			mTileX += 1;
			isCenter = true;
		}
		break;
	case characterNS::LEFT:
		if (mSpriteData.x / 32 <= mTileX - 1)
		{
			mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
			mTileX -= 1;
			isCenter = true;
		}
		break;
	case characterNS::UP:
		if (mSpriteData.y / 32 <= mTileY - 1)
		{
			mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
			mTileY -= 1;
			isCenter = true;
		}
		break;
	case characterNS::DOWN:
		if (mSpriteData.y / 32 >= mTileY + 1)
		{
			mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
			mTileY += 1;
			isCenter = true;
		}
		break;
	}
	return isCenter;
}

//==========================================================
// �w�肵�������ɐi�ݎn�߂��邩�ǂ���
//==========================================================
bool Enemy::canMoveTo(const characterNS::DIRECTION dir)
{
	// �w�肵�������̃^�C���ɏ�Q��������Έړ��ł��Ȃ�
	// �Ȃ���Έړ��\
	switch (dir)
	{
	case characterNS::LEFT:
		if (mMap->getMapCol(mTileY, mTileX - 1) >= 1)
		{
			return false;
		}
		break;
	case characterNS::RIGHT:
		if (mMap->getMapCol(mTileY, mTileX + 1) >= 1)
		{
			return false;
		}
		break;
	case characterNS::UP:
		if (mMap->getMapCol(mTileY - 1, mTileX) >= 1)
		{
			return false;
		}
		break;
	case characterNS::DOWN:
		if (mMap->getMapCol(mTileY + 1, mTileX) >= 1)
		{
			return false;
		}
		break;
	}
	return true;
}