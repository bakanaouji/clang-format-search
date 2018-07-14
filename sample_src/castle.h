//==========================================================
/// @file
/// @brief    Castle�N���X
/// @author   �������V
///
/// @attention  ���\���N���X�ł��B
///				�v���C���[�Ɠ��l�ɁA��̗̑͂�0�ɂȂ�ƃQ�[���[�I�[�o�[�ł��B

//==========================================================
#ifndef _CASTLE_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _CASTLE_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "character.h"
#include "constants.h"

//==========================================================

// Castle�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace castleNS
{
	const int WIDTH = 120;			// �摜�̕��i�e�t���[���j
	const int HEIGHT = 120;			// �摜�̍���
	const int TEXTURE_COLS = 2;		// �e�N�X�`����2��
	const int X = 549;				// �����ʒu
	const int Y = 285;
	const int START_FRAME = 1;		// �A�j���[�V�����̍ŏ��̃t���[��
}

// �v���C���[�Ɠ��l�ɁA��̗̑͂�0�ɂȂ�ƃQ�[���[�I�[�o�[�ł��B
class Castle : public Entity		// Entity�N���X���p��
{
private:
	bool mDeath;					// �邪���S����true�Bactive��false�ɂ���ƕ`�悳��Ȃ��Ȃ�̂ŁA����ɂ��̃t���O��p����
public:
	// �R���X�g���N�^
	Castle();

	//==========================================================
	// �p�����ꂽ�����o�[�֐�
	//==========================================================
	
	// Update
	// �A�j���[�V�����̍X�V���s���B
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void update(const float frameTime);
	// �_���[�W����
	// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
	// �����Fweapon	WEAPON�̎��
	void damage(const WEAPON weapon);
	// �p�����[�^���Z�b�g
	// roundStart()���ŌĂяo�����
	void reset();

	//==========================================================
	// getter
	//==========================================================

	// �邪���S���Ă��邩�ǂ�����Ԃ�
	// �߂�l�F�邪���S���Ă��邩�ǂ���
	bool isDeath() { return mDeath; }

	//==========================================================
	// setter
	//==========================================================

	// �邪���S���Ă��邩�ǂ������Z�b�g
	// �����F�邪���S���Ă��邩�ǂ���
	void setDeath(const bool death) { mDeath = death; }
};

#endif // !_CASTLE_H
