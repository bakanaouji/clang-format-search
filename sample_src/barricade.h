//==========================================================
/// @file
/// @brief    Barricade�N���X
/// @author   �������V
///
/// @attention  �o���P�[�h��\���N���X�ł��B
///				��̎���ɔz�u����A�j�󂳂��Ə�֓˔j����܂��B

//==========================================================
#ifndef _BARRICADE_H	// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _BARRICADE_H	// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "constants.h"
#include "hitEffect.h"

//==========================================================

// Barricade�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace barricadeNS
{
	const int WIDTH = 32;			// �摜�̕��i�e�t���[���j
	const int HEIGHT = 32;			// �摜�̍���
	const int TEXTURE_COLS = 8;		// �e�N�X�`����8��
	const int START_FRAME = 0;		// �A�j���[�V�����̓t���[��0����J�n
	const int END_FRAME = 2;		// �A�j���[�V�����t���[����0�A1�A2
}

//==========================================================

class Barricade : public Entity		// Entity�N���X���p��
{
private:
	HitEffect hitEffect;			// �U�����q�b�g�����Ƃ��̃A�j���[�V�����摜
public:
	// �R���X�g���N�^
	Barricade();

	//==========================================================
	// �p�����ꂽ�����o�֐�
	//==========================================================

	// Update
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// �c��̗͂ɉ����ăA�j���[�V�������X�V����B
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void update(const float frameTime);
	// �_���[�W����
	// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
	// �����Fweapon	WEAPON�̎��
	void damage(const WEAPON weapon);
	// �p�����[�^���Z�b�g
	// roundStart()���ŌĂяo�����
	virtual void reset();

	//==========================================================
	// �V�����ǉ����郁���o�[�֐�
	//==========================================================

	//==========================================================
	// setter
	//==========================================================
	
	// �_���[�W���󂯂�Ƃ��̔{�����Z�b�g
	// �����FdamagePer	�_���[�W���󂯂�Ƃ��̔{��
	void setDamagerPer(const float damagePer) { mDamagePer = damagePer; }
	
	//==========================================================
	// getter
	//==========================================================

	// �U�����q�b�g�����Ƃ��̃A�j���[�V�����摜��߂�
	// �߂�l�F�U�����q�b�g�����Ƃ��̃A�j���[�V�����摜
	HitEffect& getHitEffect() { return hitEffect; }

};
#endif // !_BARRICADE_H
