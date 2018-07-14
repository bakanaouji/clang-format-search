//==========================================================
/// @file
/// @brief    HpTextImage�N���X
/// @author   �������V
///
/// @attention  HP�Ƃ��������̉摜��\���N���X�ł��B
///				�V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p�ł��܂��B

//==========================================================
#ifndef _HP_TEXT_IMAGE_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _HP_TEXT_IMAGE_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "image.h"
#include "constants.h"

//==========================================================
// HpTextImage�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace hpTextImageNS
{
	const int WIDTH = 40;		// �摜�̕��i�e�t���[���j
	const int HEIGHT = 20;		// �摜�̍���
	const int X = 60;			// �����ʒu
	const int Y = 5;
	const int TEXTURE_COLS = 5;	// �e�N�X�`����5��
	const int START_FRAME = 15;	// �A�j���[�V�����̓t���[��15����J�n
}

//==========================================================
// HP�Ƃ��������̉摜��\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p�ł��܂��B
class HpTextImage : public Image
{
public:
	// �R���X�g���N�^
	HpTextImage();
};
#endif // !_HP_TEXT_IMAGE_H
