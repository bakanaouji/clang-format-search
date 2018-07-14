//==========================================================
/// @file
/// @brief    brave.h�̎���
/// @author   �������V
///
/// @attention  �v���C���[�����삷��E�҂�\���N���X�ł��B
///				�v���C���[����������Ĉړ��A�U���A�K�E�Z�A�K�[�h����g���ēG��|���܂��B

//==========================================================

#include "brave.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
Brave::Brave() : Character()
{
	mSpriteData.width = braveNS::WIDTH;							// �E�҂̃T�C�Y
	mSpriteData.height = braveNS::HEIGHT;
	mSpriteData.x = braveNS::X;									// ��ʏ�̈ʒu
	mSpriteData.y = braveNS::Y;
	mSpriteData.rect.bottom = braveNS::HEIGHT;					// ��ʂ̈ꕔ��I��
	mSpriteData.rect.right = braveNS::WIDTH;
	mFrameDelay = braveNS::ANIMATION_DELAY;						// �A�j���[�V�����̊e�t���[���̊Ԋu
	mStartFrame = braveNS::MOVE_UP_START_FRAME;					// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = braveNS::MOVE_UP_END_FRAME;						// �A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;								// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
	// Box�̏Փ˔���p
	mEdge.left = (LONG)(-braveNS::WIDTH * mSpriteData.scale / 2.0);
	mEdge.right = (LONG)(braveNS::WIDTH * mSpriteData.scale / 2.0);
	mEdge.top = (LONG)(-braveNS::HEIGHT * mSpriteData.scale / 2.0);
	mEdge.bottom = (LONG)(braveNS::HEIGHT * mSpriteData.scale / 2.0);

	mSecondAttackFlag = false;									// ��A���ڂ̍U���t���O�̓I�t
	mMpTimer = 0.0;												// MP�񕜗p�̃^�C�}�[
	mMagicPoint = 100;											// MP��MAX100�ŃX�^�[�g
	mMoveUpStartFrame = braveNS::MOVE_UP_START_FRAME;			// ������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveUpEndFrame = braveNS::MOVE_UP_END_FRAME;				// ������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveRightStartFrame = braveNS::MOVE_RIGHT_START_FRAME;		// �E�����ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveRightEndFrame = braveNS::MOVE_RIGHT_END_FRAME;			// �E�����ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveDownStartFrame = braveNS::MOVE_DOWN_START_FRAME;		// �������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveDownEndFrame = braveNS::MOVE_DOWN_END_FRAME;			// �������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveLeftStartFrame = braveNS::MOVE_LEFT_START_FRAME;		// �������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveLeftEndFrame = braveNS::MOVE_LEFT_END_FRAME;			// �������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mFrameDelay = braveNS::ANIMATION_DELAY;						// �A�j���[�V�����̃t���[���Ԃ̊Ԋu�i�b�j
	mAttackUpStartFrame = braveNS::UP_ATTACK_START_FRAME;		// ������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackUpEndFrame = braveNS::UP_ATTACK_END_FRAME;			// ������U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackRightStartFrame = braveNS::RIGHT_ATTACK_START_FRAME;	// �E�����U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackRightEndFrame = braveNS::RIGHT_ATTACK_END_FRAME;		// �E�����U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackDownStartFrame = braveNS::DOWN_ATTACK_START_FRAME;	// �������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackDownEndFrame = braveNS::DOWN_ATTACK_END_FRAME;		// �������U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackLeftStartFrame = braveNS::LEFT_ATTACK_START_FRAME;	// �������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackLeftEndFrame = braveNS::LEFT_ATTACK_END_FRAME;		// �������U���̃A�j���[�V�����̍Ō�̃t���[��
}

//==========================================================
// �p�����[�^������
//==========================================================
void Brave::reset()
{
	mMpTimer = 0.0;												// MP�񕜗p�̃^�C�}�[
	mMagicPoint = 100;											// MP��MAX100�ŃX�^�[�g
	mSpriteData.x = braveNS::X;									// ��ʏ�̈ʒu
	mSpriteData.y = braveNS::Y;
	mFrameDelay = braveNS::ANIMATION_DELAY;						// �A�j���[�V�����̃t���[���Ԃ̊Ԋu�i�b�j
	mStartFrame = braveNS::MOVE_UP_START_FRAME;					// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = braveNS::MOVE_UP_END_FRAME;						// �A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;								// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
	mSecondAttackFlag = false;									// ��A���ڂ̍U���t���O�̓I�t
	mFire.reset();												// ����������
	mBraveAttackCollision.reset();								// �U���G���e�B�e�B��������
	Character::reset();
}

//=============================================================================
// Update
// �L�[�{�[�h����̓��͂��󂯕t���A����ɉ����Ĉړ���U���Ȃǂ̍s�����s���B
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================	
void Brave::update(const float frameTime)
{
	// �G���e�B�e�B����A�N�e�B�u�Ȃ�A�������Ȃ�
	if (!mActive)
		return;


	// �E�҂̍U�����肪�łĂ���ꍇ�̓R���W�����𐶐����ē����蔻����Ƃ�
	if (mAttackCollisionFlag)
		mBraveAttackCollision.attack(getCenterX(), getCenterY(), (float)getWidth(), (float)getHeight(), mDirection);

	// FIRE_KEY�ɑΉ�����L�[�������ꂽ��E�҂��牊�𔭎�
	if (mInput->isKeyDown(BRAVE_FIRE_KEY))
		mFire.fire(getCenterX(), getCenterY(), (float)getWidth(), (float)getHeight(), getScale(), mMagicPoint, mDirection);

	// �U���p�̃G���e�B�e�B���o��������t���O���I�t
	mAttackCollisionFlag = false;

	// �����ŏ�ԑJ�ځA���W�̈ړ��A�A�j���[�V�����̃��Z�b�g���s��
	// ���ꂼ��̏�Ԃ��Ƃɏ����𕪊�
	switch (mState)
	{
	case characterNS::MOVE:		// �ړ����͂��ׂẴL�[�̓��͂��󂯕t����
		// �㉺���E�L�[�����͂��ꂽ�ꍇ�A
		if (mInput->isKeyDown(BRAVE_LEFT_KEY) || mInput->isKeyDown(BRAVE_RIGHT_KEY) || mInput->isKeyDown(BRAVE_UP_KEY) || mInput->isKeyDown(BRAVE_DOWN_KEY))
		{
			// ���͂ɉ����ėE�҂̌�����ύX�A�ړ�
			changeWithMove(frameTime);
		}
		if (mInput->isKeyDown(BRAVE_ATTACK_KEY))	// �U���L�[�������ꂽ�ꍇ�A
		{
			// �U���p�I�[�f�B�I���Đ�
			mAudio->playCue("kill");
			// ��Ԃ��U�����ɑJ��
			mState = characterNS::ATTACK;
			// �A�j���[�V�����t���[���̑J�ڂ͐�������
			mMode = imageNS::VERTICAL;
			// �A�j���[�V�����I�����Ƀt���[����߂����߂ɕۑ�
			mOldStartFrame = mStartFrame;
			mOldEndFrame = mEndFrame;
			// �U���p�̃G���e�B�e�B�̏o���t���O���I��
			mAttackCollisionFlag = true;
			// �����Ă�������ŃA�j���[�V�����𕪊�
			mLoop = false;
			switch (mDirection)
			{
			case characterNS::DOWN:
				mStartFrame = mAttackDownStartFrame;
				mEndFrame = mAttackDownEndFrame;
				break;
			case characterNS::RIGHT:
				mStartFrame = mAttackRightStartFrame;
				mEndFrame = mAttackRightEndFrame;
				break;
			case characterNS::LEFT:
				mStartFrame = mAttackLeftStartFrame;
				mEndFrame = mAttackLeftEndFrame;
				break;
			case characterNS::UP:
				mStartFrame = mAttackUpStartFrame;
				mEndFrame = mAttackUpEndFrame;
				break;
			}
			setCurrentFrame(mStartFrame);
		}
		else if (mInput->isKeyDown(BRAVE_GAURD_KEY))	// �K�[�h�L�[�������ꂽ�ꍇ�A
		{
			// ��Ԃ��K�[�h���ɑJ��
			mState = characterNS::GAURD;
			// �A�j���[�V�����I�����Ƀt���[����߂����߂ɕۑ�
			mOldStartFrame = mStartFrame;
			mOldEndFrame = mEndFrame;
			// �����Ă�������ŃA�j���[�V�����𕪊�
			switch (mDirection)
			{
			case characterNS::DOWN:
				mStartFrame = braveNS::DOWN_GUARD_START_FRAME;
				mEndFrame = braveNS::DOWN_GUARD_END_FRAME;
				break;
			case characterNS::RIGHT:
				mStartFrame = braveNS::RIGHT_GUARD_START_FRAME;
				mEndFrame = braveNS::RIGHT_GUARD_END_FRAME;
				break;
			case characterNS::LEFT:
				mStartFrame = braveNS::LEFT_GUARD_START_FRAME;
				mEndFrame = braveNS::LEFT_GUARD_END_FRAME;
				break;
			case characterNS::UP:
				mStartFrame = braveNS::UP_GUARD_START_FRAME;
				mEndFrame = braveNS::UP_GUARD_END_FRAME;
				break;
			}
			setCurrentFrame(mStartFrame);
		}
		break;
	case characterNS::ATTACK:	// �U�����̓A�j���[�V�������I������܂ő��i�U�����ړ��̓��͂����󂯕t���Ȃ�
		// �㉺���E�L�[�����͂��ꂽ�ꍇ�A
		if (mInput->isKeyDown(BRAVE_LEFT_KEY) || mInput->isKeyDown(BRAVE_RIGHT_KEY) || mInput->isKeyDown(BRAVE_UP_KEY) || mInput->isKeyDown(BRAVE_DOWN_KEY))
		{
			// ���͂ɉ����ėE�҂̌�����ύX�A�ړ�
			changeWithMove(frameTime);
		}
		// �U�����ɂ���ɍU���{�^���������ꂽ�ꍇ�A
		if (mInput->isKeyDown(BRAVE_ATTACK_KEY) && mCurrentFrame > mStartFrame + 2)
		{
			// ��񌂍U���̃t���O���I��
			mSecondAttackFlag = true;
		}
		// �U���A�j���[�V�������I�������ꍇ�A
		if (mAnimComplete)
		{
			// ��񌂍U���̃t���O���I���Ȃ�
			if (mSecondAttackFlag)
			{
				// ��Ԃ͑�񌂍U�����ւƑJ��
				mState = characterNS::SECOND_ATTACK;
				// ���݂̌����ɍ��킹�ăA�j���[�V�������ēx���Z�b�g
				mLoop = false;
				switch (mDirection)
				{
				case characterNS::DOWN:
					mStartFrame = braveNS::DOWN_SECOND_ATTACK_START_FRAME;
					mEndFrame = braveNS::DOWN_SECOND_ATTACK_END_FRAME;
					break;
				case characterNS::RIGHT:
					mStartFrame = braveNS::RIGHT_SECOND_ATTACK_START_FRAME;
					mEndFrame = braveNS::RIGHT_SECOND_ATTACK_END_FRAME;
					break;
				case characterNS::UP:
					mStartFrame = braveNS::UP_SECOND_ATTACK_START_FRAME;
					mEndFrame = braveNS::UP_SECOND_ATTACK_END_FRAME;
					break;
				case characterNS::LEFT:
					mStartFrame = braveNS::LEFT_SECOND_ATTACK_START_FRAME;
					mEndFrame = braveNS::LEFT_SECOND_ATTACK_END_FRAME;
					break;
				}
				setCurrentFrame(mStartFrame);
				// ��񌂍U���̃t���O���I�t
				mSecondAttackFlag = false;
				// ��񌂍U�����͌y���W�����v���Ȃ���U������
				mVelocity.y = -sqrt(2 * 2000.0f * braveNS::HEIGHT);
				// �W�����v���J�n���������Y���W��ۑ�
				mTmpY = getY();
				// �U���̃G���e�B�e�B�̏o���t���O���I��
				mAttackCollisionFlag = true;
			}
			else
			// ��񌂍U���̃t���O���I�t�Ȃ�
			{
				// ��Ԃ��ړ����ւƑJ��
				changeStateToMove();
			}
			// �A�j���[�V�����̂ݍX�V�i�G���e�B�e�B�͍X�V���Ȃ��j
			Entity::updateOnlyImage(frameTime);
		}
		break;
	case characterNS::SECOND_ATTACK:	// ���i�U�����̓A�j���[�V�������I������܂œ��͂��󂯕t���Ȃ�
		// �W�����v���Ȃ̂ŁA�d�͂ɏ]���Č���������
		mVelocity.y += frameTime * 2000.0f;
		// y�����̑��x = -�W�����v���J�n�����Ƃ��̏����x�̏ꍇ�i�W�����v�O��y���W�܂Ŗ߂��Ă��邱�Ƃ����҂����j�A
		if (mVelocity.y > sqrt(2 * 2000.0f * braveNS::HEIGHT))
		{
			// �኱�̂����␳���邽�߂ɁA�����I��y���W���W�����v�O�̍��W�ւƖ߂�
			setY(mTmpY);
			// ��Ԃ��ړ����ւƑJ��
			changeStateToMove();
			// �A�j���[�V�����̂ݍX�V�i�G���e�B�e�B�͍X�V���Ȃ��j
			Entity::updateOnlyImage(frameTime);
			// y�����ւ̑��x��0�ɂ��ăW�����v�I��
			mVelocity.y = 0.0f;
		}
		break;
	case characterNS::GAURD:	// �{�^���������ꂽ��K�[�h�I��
		if (!mInput->isKeyDown(BRAVE_GAURD_KEY))
		{
			// ��Ԃ��ړ����ւƑJ��
			changeStateToMove();
		}
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
	case characterNS::GAURD:
		Entity::update(frameTime);
		break;
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
		if (mTotalDamageTime > braveNS::DAMAGE_TIME)
		{
			mDamageTimer = 0.0f;
			mTotalDamageTime = 0.0f;
			mDrawFlag = true;
			mIsDamaged = false;
		}
	}

	// MP����莞�Ԃ��Ƃɉ�
	mMpTimer += frameTime;
	if (mMpTimer > BRAVE_MP_RECOVERY_TIME)
	{
		mMpTimer = 0.0f;
		mMagicPoint += BRAVE_MP_RECOVERY;
		// MAX100�𒴂�����A100�ɂ���
		if (mMagicPoint > 100)
			mMagicPoint = 100;
	}

	// �ړ��\��������i��񌂍U�����̓W�����v���Ȃ̂ňړ��������I�ɋ��j
	if (checkCanMove(mSpriteData.x + frameTime * mVelocity.x, mSpriteData.y + frameTime * mVelocity.y) || mState == characterNS::SECOND_ATTACK)
	{
		mSpriteData.x += frameTime * mVelocity.x;     // �L������X�����ɓ�����
		mSpriteData.y += frameTime * mVelocity.y;     // �L������Y�����ɓ�����
	}

	// ��ʂ̒[�܂ŗ�����i�߂Ȃ�
	if (mSpriteData.x > GAME_WIDTH - braveNS::WIDTH * getScale())	// ��ʉE�[�𒴂�����
		mSpriteData.x = GAME_WIDTH - braveNS::WIDTH * getScale();	// ��ʉE�[�Ɉړ�
	if (mSpriteData.x < 0)											// ��ʍ��[�𒴂�����
		mSpriteData.x = 0;											// ��ʍ��[�Ɉړ�
	if (mSpriteData.y < rectNS::HEIGHT - 10)							// ��ʏ�[�𒴂�����
		mSpriteData.y = rectNS::HEIGHT - 10;							// ��ʏ�[�Ɉړ�
	if (mSpriteData.y > GAME_HEIGHT - braveNS::HEIGHT * getScale())  // ��ʉ��[�𒴂�����
		mSpriteData.y = GAME_HEIGHT -braveNS::HEIGHT * getScale();	// ��ʉ��[�Ɉړ�


	// �E�҂̍U���R���W�������X�V
	mBraveAttackCollision.update(frameTime);
	// �����X�V
	mFire.update(frameTime);
}

//==========================================================
// �_���[�W����
// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
//==========================================================
void Brave::damage(const WEAPON weapon)
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
	case ENEMY_ATTACK:			// �G���G
		// �K�[�h���Ȃ�A�_���[�W�̑����MP������
		if (mState == characterNS::GAURD)
			mMagicPoint -= 25;
		else  // �K�[�h���łȂ��Ȃ�A
		{
			// ���_���[�W���󂯂�
			mHealth -= ENEMY_ATTACK_DAMAGE * mDamagePer;
			// �_���[�W��Ԃ̃t���O���I��
			mIsDamaged = true;
		}
		break;
	case MIDBOSS_ATTACK:		// ���{�X
		// �K�[�h���Ȃ�A�_���[�W�̑����MP������
		if (mState == characterNS::GAURD)
			mMagicPoint -= 35;
		else  // �K�[�h���łȂ��Ȃ�A
		{
			// ���_���[�W���󂯂�
			mHealth -= MIDBOSS_ATTACK_DAMAGE * mDamagePer;
			// �_���[�W��Ԃ̃t���O���I��
			mIsDamaged = true;
		}
		break;
	default:
		break;
	}
	// �̗͂�0�ȉ��ɂȂ����Ȃ�A���S
	if (mHealth <= 0)
		dead();
}

//==========================================================
// �����Ă�������ƃA�j���[�V������؂�ւ���
// �����I�ɂ̂ݎg�p
//==========================================================
void Brave::changeWithMove(const float frameTime)
{
	float r = 1.0f;
	if (mState == BRAVE_ATTACK)
		r = 2.0f;
	// ���L�[�����͂��ꂽ�ꍇ�A
	if (mInput->isKeyDown(BRAVE_LEFT_KEY))
	{
		// �����Ă������������
		setDirection(characterNS::LEFT);
		// �ړ��\��������
		if (checkCanMove(mSpriteData.x - BRAVE_MOVE_SPEED * frameTime, mSpriteData.y)) {
			// ���Ɉړ�
			mSpriteData.x -= BRAVE_MOVE_SPEED / r * frameTime;
		}
	}
	// �E�L�[�����͂��ꂽ�ꍇ�A
	if (mInput->isKeyDown(BRAVE_RIGHT_KEY))
	{
		// �����Ă���������E��
		setDirection(characterNS::RIGHT);
		// �ړ��\��������
		if (checkCanMove(mSpriteData.x + BRAVE_MOVE_SPEED * frameTime, mSpriteData.y)) {
			// �E�Ɉړ�
			mSpriteData.x += BRAVE_MOVE_SPEED / r * frameTime;
		}
	}
	// ��L�[�����͂��ꂽ�ꍇ�A
	if (mInput->isKeyDown(BRAVE_UP_KEY))
	{
		// �����Ă�����������
		setDirection(characterNS::UP);
		// �ړ��\��������
		if (checkCanMove(mSpriteData.x, mSpriteData.y - BRAVE_MOVE_SPEED * frameTime)) {
			// ��Ɉړ�
			mSpriteData.y -= BRAVE_MOVE_SPEED / r * frameTime;
		}
	}
	// ���L�[�����͂��ꂽ�ꍇ�A
	if (mInput->isKeyDown(BRAVE_DOWN_KEY))
	{
		// �����Ă������������
		setDirection(characterNS::DOWN);
		// �ړ��\��������
		if (checkCanMove(mSpriteData.x, mSpriteData.y + BRAVE_MOVE_SPEED * frameTime)) {
			// ���Ɉړ�
			mSpriteData.y += BRAVE_MOVE_SPEED / r * frameTime;
		}
	}
	// �A�j���[�V�����̂ݍX�V�i�G���e�B�e�B�͍X�V���Ȃ��j
	Entity::updateOnlyImage(frameTime);
}