//==========================================================
/// @file
/// @brief    midBoss.h�̎���
/// @author   �������V
///
/// @attention  ���{�X��\���N���X�ł��B
///				Enemy�N���X���p�����Ă��邽�߁A����͂قƂ�ǎG���G�ƈꏏ�ł��B

#include "midBoss.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
MidBoss::MidBoss() : Enemy()
{
	// �����̕����͉E
	mStartFrame = midBossNS::MOVE_RIGHT_START_FRAME;				// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = midBossNS::MOVE_RIGHT_END_FRAME;					// �A�j���[�V�����̍Ō�̃t���[��
	mOldStartFrame = mStartFrame;									// �U���A�j���[�V�����I����ɖ߂�ׂ��A�j���[�V�����̍ŏ��̃t���[��
	mOldEndFrame = mEndFrame;										// �U���A�j���[�V�����I����ɖ߂�ׂ��A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;									// ���݂̃t���[�����A�j���[�V�����̍ŏ��̃t���[���ɃZ�b�g
	mAttackAnimationMode = imageNS::HORIZONTAL;					// �U�����̃A�j���[�V�����̑J�ڃ��[�h�i�����A�����j
	mMoveUpStartFrame = midBossNS::MOVE_UP_START_FRAME;			// ������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveUpEndFrame = midBossNS::MOVE_UP_END_FRAME;				// ������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveRightStartFrame = midBossNS::MOVE_RIGHT_START_FRAME;	// �E�����ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveRightEndFrame = midBossNS::MOVE_RIGHT_END_FRAME;		// �E�����ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveDownStartFrame = midBossNS::MOVE_DOWN_START_FRAME;		// �������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveDownEndFrame = midBossNS::MOVE_DOWN_END_FRAME;			// �������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mMoveLeftStartFrame = midBossNS::MOVE_LEFT_START_FRAME;		// �������ړ��̃A�j���[�V�����̍ŏ��̃t���[��
	mMoveLeftEndFrame = midBossNS::MOVE_LEFT_END_FRAME;			// �������ړ��̃A�j���[�V�����̍Ō�̃t���[��
	mAttackUpStartFrame = midBossNS::ATTACK_UP_START_FRAME;		// ������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackUpEndFrame = midBossNS::ATTACK_UP_END_FRAME;			// ������U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackRightStartFrame = midBossNS::ATTACK_RIGHT_START_FRAME;// �E�����U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackRightEndFrame = midBossNS::ATTACK_RIGHT_END_FRAME;	// �E�����U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackDownStartFrame = midBossNS::ATTACK_DOWN_START_FRAME;	// �������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackDownEndFrame = midBossNS::ATTACK_DOWN_END_FRAME;		// �������U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackLeftStartFrame = midBossNS::ATTACK_LEFT_START_FRAME;	// �������U���̃A�j���[�V�����̍ŏ��̃t���[��
	mAttackLeftEndFrame = midBossNS::ATTACK_LEFT_END_FRAME;		// �������U���̃A�j���[�V�����̍Ō�̃t���[��
	mAttackTime = midBossNS::ATTACK_TIME;						// �v���C���[�ɗאڂ��Ă���U���Ɉڂ�܂ł̗��ߎ���
	mContinueAttackTime = midBossNS::CONTINUE_ATTACK_TIME;		// �A���ōU������ꍇ�̗��ߎ���
}

//==========================================================
// �_���[�W����
// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
//==========================================================
void MidBoss::damage(const WEAPON weapon)
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
		mHealth -= BRAVE_FIRE_DAMAGE  * mDamagePer / 3;
		// �_���[�W��Ԃ̃t���O���I��
		mIsDamaged = true;
		break;
	case BRAVE_ATTACK:			// �E�҂̍U��
		// ���_���[�W���󂯂�
		mHealth -= BRAVE_ATTACK_DAMAGE * mDamagePer / 3;
		// �_���[�W��Ԃ̃t���O���I��
		mIsDamaged = true;
		break;
	case BRAVE_SECOND_ATTACK:	// �E�҂̑��
		// ���_���[�W���󂯂�
		mHealth -= BRAVE_ATTACK_DAMAGE * mDamagePer / 3;
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