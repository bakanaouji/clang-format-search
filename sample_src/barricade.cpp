//==========================================================
/// @file
/// @brief    barricade.h�̎���
/// @author   �������V
///
/// @attention  �o���P�[�h��\���N���X�ł��B
///				��̎���ɔz�u����A�j�󂳂��Ə�֓˔j����܂��B

//==========================================================
#include "barricade.h"

//==========================================================
// �f�t�H���g�R���X�g���N�^
//==========================================================
Barricade::Barricade() : Entity()
{
	mSpriteData.width = barricadeNS::WIDTH;			// �P�̉摜�̃T�C�Y
	mSpriteData.height = barricadeNS::HEIGHT;
	mSpriteData.rect.bottom = barricadeNS::HEIGHT;	// �摜���̑I�����镔��
	mSpriteData.rect.right = barricadeNS::WIDTH;
	mCols = barricadeNS::TEXTURE_COLS;				// �e�N�X�`���̗�
	mStartFrame = barricadeNS::START_FRAME;			// �A�j���[�V�����̍ŏ��̃t���[��
	mEndFrame = barricadeNS::END_FRAME;				// �A�j���[�V�����̍Ō�̃t���[��
	mCurrentFrame = mStartFrame;						// ���݂̃t���[���̓A�j���[�V�����̍ŏ��̃t���[���ɐݒ�
	// Box�̏Փ˔���p
	mEdge.left = (LONG)(-barricadeNS::WIDTH / 2.0);
	mEdge.right = (LONG)(barricadeNS::WIDTH / 2.0);
	mEdge.top = (LONG)(-barricadeNS::HEIGHT / 2.0);
	mEdge.bottom = (LONG)(barricadeNS::HEIGHT / 2.0);
	mCollisionType = entityNS::BOX;
}

//==========================================================
// Update
// �c��̗͂ɉ����ăA�j���[�V�������X�V����B
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//==========================================================
void Barricade::update(const float frameTime)
{
	// ��A�N�e�B�u�ȏꍇ�A�������Ȃ�
	if (mActive == false)
		return;

	// �c��̗͂��ƂɁA�A�j���[�V�����𕪊�
	// ���E���c��̗͂��㉺�����ꍇ�A�A�j���[�V������J�ڂ�����
	if ((mCurrentFrame == barricadeNS::START_FRAME && mHealth < 66) || (mCurrentFrame == barricadeNS::START_FRAME + 1 && mHealth < 33))
	{
		// �̗͂����E����������ꍇ�A�A�j���[�V������i�߂�
		setCurrentFrame(mCurrentFrame + 1);
	}
	if ((mCurrentFrame == barricadeNS::END_FRAME && mHealth >= 33) || (mCurrentFrame == barricadeNS::START_FRAME + 1 && mHealth >= 66))
	{
		// �̗͂����E���������ꍇ�A�A�j���[�V������߂�
		setCurrentFrame(mCurrentFrame - 1);
	}
	// �U�����q�b�g�����Ƃ��̃A�j���[�V�������X�V
	hitEffect.update(frameTime);
	// �A�j���[�V�����ȊO���X�V
	updateWithoutImage(frameTime);
	
}

//==========================================================
// �p�����[�^���Z�b�g
//==========================================================
void Barricade::reset()
{
	Entity::reset();												// �G���e�B�e�B��������
	hitEffect.setCurrentFrame(hitEffect.getStartFrame());			// �U�����q�b�g�����Ƃ��̃A�j���[�V������������
	hitEffect.setVisible(false);									// �U�����q�b�g�����Ƃ��̃A�j���[�V�����͍ŏ��͕\�����Ȃ�
}

//==========================================================
// �_���[�W����
// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
//==========================================================
void Barricade::damage(const WEAPON weapon)
{
	// ��A�N�e�B�u�ȏꍇ�A�_���[�W�͎󂯂Ȃ�
	if (!mActive)
		return;
	
	// WEAPON�ɂ���ă_���[�W�𕪊�
	switch (weapon)
	{
	case FIRE:					// ���i�v���C���[�̕K�E�Z�j
		// �ω��Ȃ�
		break;
	case BRAVE_ATTACK:			// �v���C���[�̍U��
	case BRAVE_SECOND_ATTACK:	// �v���C���[�̍U��
		// �o���P�[�h����
		mHealth += BRAVE_RECOVERY_TO_BARRICADE;
		// MAX100�𒴂����ꍇ��100�ɂ���
		if (mHealth > 100)
		{
			mHealth = 100;
		}
		// �U�����q�b�g�����Ƃ��̃A�j���[�V�����𔭓�
		hitEffect.hit(getCenterX(), getCenterY());
		break;
	case ENEMY_ATTACK:
		// �o���P�[�h�Ƀ_���[�W
		mHealth -= ENEMY_ATTACK_DAMEGE_BARRICADE * mDamagePer;
		// �U�����q�b�g�����Ƃ��̃A�j���[�V�����𔭓�
		hitEffect.hit(getCenterX(), getCenterY());
		// �c��̗͂��Ȃ��Ȃ����ꍇ�A
		if (mHealth <= 0)
		{
			// ���g�ƍU�����q�b�g�����Ƃ��̃A�j���[�V�������A�N�e�B�u�ɂ���
			mVisible = false;
			mActive = false;
			hitEffect.setVisible(false);
		}
		break;
	case MIDBOSS_ATTACK:
		// �o���P�[�h�Ƀ_���[�W
		mHealth -= MIDBOSS_ATTACK_DAMEGE_BARRICADE * mDamagePer;
		// �U�����q�b�g�����Ƃ��̃A�j���[�V�����𔭓�
		hitEffect.hit(getCenterX(), getCenterY());
		// �c��̗͂��Ȃ��Ȃ����ꍇ�A
		if (mHealth <= 0)
		{
			// ���g�ƍU�����q�b�g�����Ƃ��̃A�j���[�V�������A�N�e�B�u�ɂ���
			mVisible = false;
			mActive = false;
			hitEffect.setVisible(false);
		}
		break;
	default:
		break;
	}
}