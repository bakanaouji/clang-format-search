//==========================================================
/// @file
/// @brief    Fire�N���X
/// @author   �������V
///
/// @attention  �v���C���[�̕K�E�Z�ɂ���Ĕ��˂���鉊��\���N���X�ł��B

//==========================================================
#ifndef _FIRE_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _FIRE_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "character.h"
#include "constants.h"

//==========================================================

// Fire�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace fireNS
{
	const int WIDTH = 24;							// �摜�̕��i�e�t���[���j
	const int HEIGHT = 32;							// �摜�̍���
	const int COLLISION_RADIUS = 4;					// �~�̏Փ˔���
	const float SPEED = 300;						// 1�b������̃s�N�Z����
	const float FIRE_DELAY = 0.5f;					// �������̊Ԋu��4�b
	const int TEXTURE_COLS = 12;					// �e�N�X�`����8��
	const int START_FRAME = 52;						// �A�j���[�V�����̓t���[��52����J�n
	const int END_FRAME = 52 + 36;					// �A�j���[�V�����t���[����52�A64�A76�A88
	const float ANIMATION_DELAY = 0.1f;				// �t���[���Ԃ̎���
	const int FIRE_MP = 20;							// �K�E�Z�Ɏg�p����MP
}

// �v���C���[�̕K�E�Z�ɂ���Ĕ��˂���鉊��\���N���X�ł��B
class Fire : public Entity							// Entity�N���X���p��
{
private:
	float mFireTimer;								// ���˂��\�ɂȂ�܂ł̎c�莞��
public:
	// �R���X�g���N�^
	Fire();

	//==========================================================
	// �p�����ꂽ�����o�[�֐�
	//==========================================================

	// Update
	// �G���e�B�e�B���A�N�e�B�u�ȏꍇ�A��莞�Ԃ܂������ړ�
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void update(const float frameTime);
	// �p�����[�^���Z�b�g
	// roundStart()���ŌĂяo�����
	void reset();

	// �V���������o�[�֐�
	// �E�҂��牊�𔭎�
	// �w�肵���ʒu�ɂ��̃G���e�B�e�B���A�N�e�B�u�ɂ��ďo��������
	// �����FcenterX	�U�����s�����G���e�B�e�B�̒��S��X�����̈ʒu
	// �����FcenterY	�U�����s�����G���e�B�e�B�̒��S��Y�����̈ʒu
	// �����Fwidth		�U�����s�����G���e�B�e�B�̕�
	// �����Fheight		�U�����s�����G���e�B�e�B�̍���
	// �����Fscale		�U�����s�����G���e�B�e�B�̃X�P�[��
	// �����Fmp			�U�����s�����G���e�B�e�B�̎c��MP
	// �����Fdirection	�U�����s�����G���e�B�e�B�������Ă������
	void fire(const float centerX, const float centerY, const float width, const float height, const float scale, int& mp, const characterNS::DIRECTION direction);
};
#endif
