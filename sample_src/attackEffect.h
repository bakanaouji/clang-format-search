//==========================================================
/// @file
/// @brief    AttackEffect�N���X
/// @author   �������V
///
/// @attention  �U�����̃A�j���[�V������\������N���X�ł��B

//==========================================================
#ifndef _ATTACK_EFFECT_H	// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�A
#define _ATTACK_EFFECT_H	// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "image.h"
#include "character.h"

//==========================================================

// AttackEffect�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace attackEffectNS
{
	const int WIDTH = 96;					// �摜�̕��i�e�t���[���j
	const int HEIGHT = 96;					// �摜�̍���
	const int TEXTURE_COLS = 5;				// �e�N�X�`����5��
	const int START_FRAME = 0;				// �A�j���[�V�����̓t���[��0����J�n
	const int END_FRAME = 3;				// �A�j���[�V�����t���[����0�A1�A2�A3
	const float ANIMATION_DELAY = 0.05f;	// �A�j���[�V�����̃t���[���Ԃ̎���
	const float SCALE = 0.8f;				// �X�v���C�g�̃X�P�[��
}

//==========================================================
// �U�����A�j���[�V������\������N���X�ł��B
class AttackEffect : public Image
{
public:
	// �R���X�g���N�^
	AttackEffect();


	//==========================================================
	// �p�����ꂽ�����o�֐�
	//==========================================================
	
	// Update
	// �A�j���[�V�������X�V����B
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void update (const float frameTime);

	//==========================================================
	// �V�����ǉ����郁���o�[�֐�
	//==========================================================

	// Attack
	// �U���̏Ռ��g�̃A�j���[�V�������w�肳�ꂽ�ʒu�ɔ���
	// �����FcenterX	�U�����s�����G���e�B�e�B�̒��S��X�����̈ʒu
	// �����FcenterY	�U�����s�����G���e�B�e�B�̒��S��Y�����̈ʒu
	// �����Fwidth		�U�����s�����G���e�B�e�B�̕�
	// �����Fheight		�U�����s�����G���e�B�e�B�̍���
	// �����Fdirection	�U�����s�����G���e�B�e�B�������Ă������
	void attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction);
};
#endif // !_ATTACK_EFFECT_H
