//==========================================================
/// @file
/// @brief    MidBoss�N���X
/// @author   �������V
///
/// @attention  ���{�X��\���N���X�ł��B
///				Enemy�N���X���p�����Ă��邽�߁A����͂قƂ�ǎG���G�ƈꏏ�ł��B

//==========================================================
#ifndef _MID_BOSS_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _MID_BOSS_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "enemy.h"

//==========================================================
// MidBoss�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace midBossNS
{
	const int MOVE_UP_START_FRAME = 0;				// ������ړ��̃A�j���[�V�����̓t���[��0����J�n
	const int MOVE_UP_END_FRAME = 2;				// ������ړ��̃A�j���[�V�����t���[����0�A1�A2
	const int MOVE_RIGHT_START_FRAME = 12;			// �E�����ړ��̃A�j���[�V�����̓t���[��12����J�n
	const int MOVE_RIGHT_END_FRAME = 14;			// �E�����ړ��̃A�j���[�V�����t���[����12�A13�A14
	const int MOVE_DOWN_START_FRAME = 24;			// �������ړ��̃A�j���[�V�����̓t���[��24����J�n
	const int MOVE_DOWN_END_FRAME = 26;				// �������ړ��̃A�j���[�V�����t���[����24�A25�A26
	const int MOVE_LEFT_START_FRAME = 36;			// �������ړ��̃A�j���[�V�����̓t���[��36����J�n
	const int MOVE_LEFT_END_FRAME = 38;				// �������ړ��̃A�j���[�V�����t���[����36�A37�A38
	const int ATTACK_DOWN_START_FRAME = 96;			// ������U���̃A�j���[�V�����̓t���[��96����J�n
	const int ATTACK_DOWN_END_FRAME = 98;			// ������U���ɃA�j���[�V�����t���[����28�A29�A175�A184
	const int ATTACK_LEFT_START_FRAME = 99;			// �E�����U���̃A�j���[�V�����̓t���[��148����J�n
	const int ATTACK_LEFT_END_FRAME = 101;			// �E�����U���̃A�j���[�V�����t���[����148�A161�A173�A182
	const int ATTACK_UP_START_FRAME = 102;			// �������U���̃A�j���[�V�����̓t���[��103����J�n
	const int ATTACK_UP_END_FRAME = 104;			// �������U���̃A�j���[�V�����t���[����103�A115�A127�A139
	const int ATTACK_RIGHT_START_FRAME = 105;		// �������U���̃A�j���[�V�����̓t���[��100����J�n
	const int ATTACK_RIGHT_END_FRAME = 107;			// �������U���̃A�j���[�V�����t���[����100�A112�A124�A136
	const float ATTACK_TIME = 1.5f;					// �v���C���[�ɗאڂ��Ă���U���Ɉڂ�܂ł̗��ߎ���
	const float CONTINUE_ATTACK_TIME = 2.0f;		// �A���ōU������ꍇ�̗��ߎ���	
}

//==========================================================
// ���{�X��\���N���X�ł��B
// Enemy�N���X���p�����Ă��邽�߁A����͂قƂ�ǎG���G�ƈꏏ�ł��B
class MidBoss : public Enemy
{
public:
	// �R���X�g���N�^
	MidBoss();

	//==========================================================
	// �p�����ꂽ�����o�[�֐�
	//==========================================================
	
	// �_���[�W����
	// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
	// �����Fweapon	WEAPON�̎��
	void damage(const WEAPON weapon);
};

#endif // !_MID_BOSS_H
