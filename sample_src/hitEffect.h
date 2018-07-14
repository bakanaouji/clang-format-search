//==========================================================
/// @file
/// @brief    HitEffect�N���X
/// @author   �������V
///
/// @attention  �U�����q�b�g�����Ƃ��̃A�j���[�V������\������N���X�ł��B

//==========================================================
#ifndef _HIT_EFFECT_H	// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _HIT_EFFECT_H	// ���d�ɒ�`����邱�Ƃ�h���܂��B
//==========================================================

#include "image.h"
#include "constants.h"

// HitEffect�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace hitEffectNS
{
	const int WIDTH = 108;				// �摜�̕��i�e�t���[���j
	const int HEIGHT = 108;				// �摜�̍���
	const int TEXTURE_COLS = 3;			// �e�N�X�`����3��
	const int START_FRAME = 0;			// �A�j���[�V�����̓t���[��0����J�n
	const int END_FRAME = 2;			// �A�j���[�V�����t���[����0�A1�A2
	const float ANIMATION_DELAY = 0.1f;	// �A�j���[�V�����̃t���[���Ԃ̎���
}

// �U�����q�b�g�����Ƃ��̃A�j���[�V������\������N���X�ł��B
class HitEffect : public Image
{
public:
	// �R���X�g���N�^
	HitEffect();

	//==========================================================
	// �p�����ꂽ�����o�֐�
	//==========================================================

	// Update
	// �A�j���[�V�������X�V����B
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void update(const float frameTime);

	// Hit
	// �U���q�b�g�̃A�j���[�V�������w�肳�ꂽ���W�̎l���Ƀ����_���ɕ\��
	// �����Fx	�A�j���[�V�����̈ʒuX
	// �����Fy	�A�j���[�V�����̈ʒuY
	void hit(const float x, const float y);
};
#endif // !_ATTACK_EFFECT_H
