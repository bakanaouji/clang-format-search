//==========================================================
/// @file
/// @brief    BraveAttackCollision�N���X
/// @author   �������V
///
/// @attention  �v���C���[�i�E�ҁj�ɂ��U���̓����蔻��p�̃G���e�B�e�B���o��������N���X�ł��B

//==========================================================
#ifndef _ATTACK_COLLISION_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _ATTACK_COLLISION_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "constants.h"
#include "attackEffect.h"

//==========================================================
// BraveAttackCollision�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace braveAttackCollisionNS
{
	const int WIDTH = 24;				// �摜�̕��i�e�t���[���j
	const int HEIGHT = 96;				// �摜�̍���
	const float COLLISION_TIME = 0.3f;	// �U���̓����蔻�肪�o�Ă��鎞��
}

//==========================================================
// �v���C���[�i�E�ҁj�ɂ��U���̓����蔻��p�̃G���e�B�e�B���o��������N���X�ł��B
class BraveAttackCollision : public Entity
{
private:
	float mCollisionTimer;			// �U���̓����蔻�肪�o�Ă��鎞�Ԃ��v������^�C�}�[
	AttackEffect mAttackEffect;		// �U���̏Ռ��h
public:
	// �R���X�g���N�^
	BraveAttackCollision();

	//==========================================================
	// �p�����ꂽ�����o�[�֐�
	//==========================================================

	// Update
	// �U���̃A�j���[�V�����Ǝ��g�̃G���e�B�e�B���X�V����B
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void update(const float frameTime);
	// �p�����[�^���Z�b�g
	// roundStart()���ŌĂяo�����
	virtual void reset();

	//==========================================================
	// �V���������o�[�֐�
	//==========================================================
	
	// Attack
	// �E�҂���U���𔭓�
	// �w�肵���ʒu�ɂ��̃G���e�B�e�B���A�N�e�B�u�ɂ��ďo��������
	// �����FcenterX	�U�����s�����G���e�B�e�B�̒��S��X�����̈ʒu
	// �����FcenterY	�U�����s�����G���e�B�e�B�̒��S��Y�����̈ʒu
	// �����Fwidth		�U�����s�����G���e�B�e�B�̕�
	// �����Fheight		�U�����s�����G���e�B�e�B�̍���
	// �����Fdirection	�U�����s�����G���e�B�e�B�������Ă������
	void attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction);

	//==========================================================
	// getter
	//==========================================================

	// �U���̏Ռ��h�̉摜��߂�
	// �߂�l�F�U���p�A�j���[�V�����̉摜
	AttackEffect& getAttackEffect() { return mAttackEffect; }
};

#endif // !_ATTACK_COLLISION_H
