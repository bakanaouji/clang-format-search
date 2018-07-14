//==========================================================
/// @file
/// @brief    enemyAttackCollision.h�̎���
/// @author   �������V
///
/// @attention  �G�ɂ��U���̓����蔻��p�̃G���e�B�e�B���o��������N���X�ł��B

//==========================================================

#include "enemyAttackCollision.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
EnemyCollision::EnemyCollision() : Entity()
{
	mActive = false;												// �U���̓A�N�e�B�u�łȂ���Ԃ���J�n
	mVisible = false;
	mSpriteData.width = enemyCollisionNS::ATTACK_WIDTH;			// �P�̉摜�̃T�C�Y
	mSpriteData.height = enemyCollisionNS::ATTACK_HEIGHT;
	mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_HEIGHT;	// ��ʂ̈ꕔ��I��
	mSpriteData.rect.right = enemyCollisionNS::ATTACK_WIDTH;
	// Box�̏Փ˔���p
	mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
	mEdge.right = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
	mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
	mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
	mCollisionType = entityNS::BOX;
	mCollisionTimer = 0.0f;										// �U���̓����蔻�肪�o�Ă��鎞�Ԃ��v������^�C�}�[
}

//==========================================================
// Update
// �U���̃A�j���[�V�����Ǝ��g�̃G���e�B�e�B���X�V����B
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//==========================================================
void EnemyCollision::update(const float frameTime)
{
	// ��A�N�e�B�u�Ȃ�A���̃G���e�B�e�B�̍X�V�͂��Ȃ�
	if (mVisible == false)
		return;
	// �U���̓����蔻�肪�o�Ă��鎞�Ԃ����Z
	mCollisionTimer += frameTime;
	// �U���̓����蔻�肪���ȏ�o�Ă���Ȃ�A���̃G���e�B�e�B���A�N�e�B�u�ɂ���
	if (mCollisionTimer >= enemyCollisionNS::COLLISION_TIME)
	{
		mCollisionTimer = 0.0f;
		mVisible = false;
		mActive = false;
	}
	// �G���e�B�e�B���X�V
	Entity::update(frameTime);
}

//==========================================================
// Attack
// �G����U���𔭓�
// �w�肵���ʒu�ɂ��̃G���e�B�e�B���A�N�e�B�u�ɂ��ďo��������
//==========================================================
void EnemyCollision::attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction)
{
	// �G�̌����ɂ���ďo���ʒu�𕪊�
	// �G�̉摜�ɕ΂肪���邽�߁A�኱�ʒu���C�����Ă���
	switch (direction)
	{
	case characterNS::RIGHT:	// �E
		mSpriteData.width = enemyCollisionNS::ATTACK_WIDTH;	// �P�̉摜�̃T�C�Y
		mSpriteData.height = enemyCollisionNS::ATTACK_HEIGHT;
		mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_HEIGHT;
		mSpriteData.rect.right = enemyCollisionNS::ATTACK_WIDTH;
		// Box�̏Փ˔���p
		mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.right = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mSpriteData.x = centerX + width / 2;
		mSpriteData.y = centerY - height / 2;
		break;
	case characterNS::UP:		// ��
		mSpriteData.width = enemyCollisionNS::ATTACK_HEIGHT;	// �P�̉摜�̃T�C�Y
		mSpriteData.height = enemyCollisionNS::ATTACK_WIDTH;
		mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_WIDTH;
		mSpriteData.rect.right = enemyCollisionNS::ATTACK_HEIGHT;
		// Box�̏Փ˔���p
		mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.right = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mSpriteData.x = centerX - width / 2;
		mSpriteData.y = centerY - height / 2 - mSpriteData.height - 10;
		break;
	case characterNS::LEFT:		// ��
		mSpriteData.width = enemyCollisionNS::ATTACK_WIDTH;	// �P�̉摜�̃T�C�Y
		mSpriteData.height = enemyCollisionNS::ATTACK_HEIGHT;
		mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_HEIGHT;
		mSpriteData.rect.right = enemyCollisionNS::ATTACK_WIDTH;
		// Box�̏Փ˔���p
		mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.right = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mSpriteData.x = centerX - width / 2 - mSpriteData.width;
		mSpriteData.y = centerY - height / 2;
		break;
	case characterNS::DOWN:		// ��
		mSpriteData.width = enemyCollisionNS::ATTACK_HEIGHT;	// �P�̉摜�̃T�C�Y
		mSpriteData.height = enemyCollisionNS::ATTACK_WIDTH;
		mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_WIDTH;
		mSpriteData.rect.right = enemyCollisionNS::ATTACK_HEIGHT;
		// Box�̏Փ˔���p
		mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.right = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mSpriteData.x = centerX - width / 2;
		mSpriteData.y = centerY + height / 2;
		break;
	}
	// �U���̓����蔻�肪�o�Ă��鎞�Ԃ����Z�b�g
	mCollisionTimer = 0.0f;
	//visible = true;							// �U����\��
	mActive = true;							// �Փˉ\�ɂ���	
	setRect();
}