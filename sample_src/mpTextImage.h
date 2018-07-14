//==========================================================
/// @file
/// @brief    MpTextImage�N���X
/// @author   �������V
///
/// @attention  MP�Ƃ��������̉摜��\���N���X�ł��B
///				�V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p�ł��܂��B

//==========================================================
#ifndef _MP_TEXT_IMAGE_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _MP_TEXT_IMAGE_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "image.h"
#include "constants.h"

//==========================================================
// MpTextImage�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace mpTextImageNS
{
	const int WIDTH = 40;		// �摜�̕��i�e�t���[���j
	const int HEIGHT = 20;		// �摜�̍���
	const int X = 60;			// �����ʒu
	const int Y = 30;
	const int TEXTURE_COLS = 5;	// �e�N�X�`����5��
	const int START_FRAME = 20;	// �A�j���[�V�����̓t���[��15����J�n
}

//==========================================================
// MP�Ƃ��������̉摜��\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p�ł��܂��B
class MpTextImage : public Image
{
public:
	// �R���X�g���N�^
	MpTextImage();
};
#endif // !_MP_TEXT_IMAGE_H
