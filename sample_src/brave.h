//==========================================================
/// @file
/// @brief    Brave�N���X
/// @author   �������V
///
/// @attention  �v���C���[�����삷��E�҂�\���N���X�ł��B
///				�v���C���[����������Ĉړ��A�U���A�K�E�Z�A�K�[�h����g���ēG��|���܂��B

//==========================================================
#ifndef _BRAVE_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _BRAVE_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "constants.h"
#include "map.h"
#include "fire.h"
#include "rect.h"
#include "braveAttackCollision.h"

//==========================================================

// Brave�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace braveNS
{
	const int WIDTH = 32;							// �摜�̕��i�e�t���[���j
	const int HEIGHT = 32;							// �摜�̍���
	const int X = GAME_WIDTH / 2 - WIDTH / 2;		// ��ʏ�̈ʒu
	const int Y = GAME_HEIGHT / 2 - HEIGHT * 3;
	const int TEXTURE_COLS = 12;					// �e�N�X�`����12��
	const int MOVE_UP_START_FRAME = 48;				// ������ړ��̃A�j���[�V�����̓t���[��0����J�n
	const int MOVE_UP_END_FRAME = 50;				// ������ړ��̃A�j���[�V�����t���[����0�A1�A2�A3�A4�A5
	const int MOVE_RIGHT_START_FRAME = 60;			// �E�����ړ��̃A�j���[�V�����̓t���[��12����J�n
	const int MOVE_RIGHT_END_FRAME = 62;			// �E�����ړ��̃A�j���[�V�����t���[����12�A13�A14�A15�A16�A17
	const int MOVE_DOWN_START_FRAME = 72;			// �������ړ��̃A�j���[�V�����̓t���[��24����J�n
	const int MOVE_DOWN_END_FRAME = 74;				// �������ړ��̃A�j���[�V�����t���[����24�A25�A26�A27�A28�A29
	const int MOVE_LEFT_START_FRAME = 84;			// �������ړ��̃A�j���[�V�����̓t���[��36����J�n
	const int MOVE_LEFT_END_FRAME = 86;				// �������ړ��̃A�j���[�V�����t���[����36�A37�A38�A39�A40�A41
	const float ANIMATION_DELAY = 0.2f;				// �ړ��A�j���[�V�����̃t���[���Ԃ̎���
	const int DOWN_ATTACK_START_FRAME = 145 + 24;	// �������ւ̍U���A�j���[�V�����̓t���[��169����J�n
	const int DOWN_ATTACK_END_FRAME = 145 + 36;		// �������ւ̍U���A�j���[�V�����t���[����169�A181
	const int LEFT_ATTACK_START_FRAME = 148 + 24;	// �������ւ̍U���A�j���[�V�����̓t���[��172����J�n
	const int LEFT_ATTACK_END_FRAME = 148 + 36;		// �������ւ̍U���A�j���[�V�����t���[����172�A184
	const int UP_ATTACK_START_FRAME = 151 + 24;		// ������ւ̍U���A�j���[�V�����̓t���[��175����J�n
	const int UP_ATTACK_END_FRAME = 151 + 36;		// ������ւ̍U���A�j���[�V�����t���[����175�A187
	const int RIGHT_ATTACK_START_FRAME = 154 + 24;	// �E�����ւ̍U���A�j���[�V�����̓t���[��178����J�n
	const int RIGHT_ATTACK_END_FRAME = 154 + 36;	// �E�����ւ̍U���A�j���[�V�����t���[����178�A190
	const int UP_GUARD_START_FRAME = 240;			// ������ւ̃K�[�h�A�j���[�V�����̓t���[��240����J�n
	const int UP_GUARD_END_FRAME = 242;				// ������ւ̃K�[�h�A�j���[�V�����̓t���[��240�A241�A242
	const int RIGHT_GUARD_START_FRAME = 252;		// �E�����ւ̃K�[�h�A�j���[�V�����̓t���[��252����J�n
	const int RIGHT_GUARD_END_FRAME = 254;			// �E�����ւ̃K�[�h�A�j���[�V�����̓t���[��252�A253�A234
	const int DOWN_GUARD_START_FRAME = 264;			// �������ւ̃K�[�h�A�j���[�V�����̓t���[��264����J�n
	const int DOWN_GUARD_END_FRAME = 266;			// �������ւ̃K�[�h�A�j���[�V�����̓t���[��264�A265�A266
	const int LEFT_GUARD_START_FRAME = 276;			// �������ւ̃K�[�h�A�j���[�V�����̓t���[��276����J�n
	const int LEFT_GUARD_END_FRAME = 278;			// �������ւ̃K�[�h�A�j���[�V�����̓t���[��276�A277�A278
	const int DOWN_SECOND_ATTACK_START_FRAME = DOWN_ATTACK_START_FRAME;	// �������ւ̍U���A�j���[�V�����i���i�j�̓t���[��145����J�n
	const int DOWN_SECOND_ATTACK_END_FRAME = DOWN_ATTACK_END_FRAME;
	const int LEFT_SECOND_ATTACK_START_FRAME = LEFT_ATTACK_START_FRAME;	// �������ւ̍U���A�j���[�V�����i���i�j�̓t���[��145����J�n
	const int LEFT_SECOND_ATTACK_END_FRAME = LEFT_ATTACK_END_FRAME;
	const int UP_SECOND_ATTACK_START_FRAME = UP_ATTACK_START_FRAME;		// ������ւ̍U���A�j���[�V�����i���i�j�̓t���[��145����J�n
	const int UP_SECOND_ATTACK_END_FRAME = UP_ATTACK_END_FRAME;
	const int RIGHT_SECOND_ATTACK_START_FRAME = RIGHT_ATTACK_START_FRAME;// �E�����ւ̍U���A�j���[�V�����i���i�j�̓t���[��145����J�n
	const int RIGHT_SECOND_ATTACK_END_FRAME = RIGHT_ATTACK_END_FRAME;
	const float DAMAGE_TIME = 1.0f;					// �_���[�W���󂯂ē_�ł��鎞��
}

//==========================================================
// �v���C���[�����삷��E�҂�\���N���X�ł��B
class Brave : public Character
{
private:
	int mMagicPoint;							// MP�A�K�E�Z���g���ƌ����B
	float mMpTimer;								// MP�񕜗p�̃^�C�}�[
	float mTmpY;								// �ꎞ�I��Y���W�ۑ��p
	bool mSecondAttackFlag;						// ���i�U���A�j���[�V�����̊J�n�t���O
	Fire mFire;									// ��
	BraveAttackCollision mBraveAttackCollision;	// �E�҂̍U���̓����蔻��p
	// �����Ă�������ƃA�j���[�V������؂�ւ���
	// �����I�ɂ̂ݎg�p
	void changeWithMove(const float frameTime);
public:
	// �R���X�g���N�^
	Brave();

	//==========================================================
	// �p�����ꂽ�����o�[�֐�
	//==========================================================

	// Update
	// �L�[�{�[�h����̓��͂��󂯕t���A����ɉ����Ĉړ���U���Ȃǂ̍s�����s���B
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void update(const float frameTime);
	// �l�H�m�\�BNPC�̍s�������肷��̂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	// �����Fent		���̃G���e�B�e�B
	void ai(const float frameTime, const Entity &ent) {};
	// �_���[�W����
	// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
	// �����Fweapon	WEAPON�̎��
	void damage(const WEAPON weapon);
	// �p�����[�^���Z�b�g
	// roundStart()���ŌĂяo�����
	void reset();

	//==========================================================
	// �V�����ǉ����郁���o�[�֐�
	//==========================================================

	//==========================================================
	// getter
	//==========================================================
	
	// MP��Ԃ��֐�
	// �߂�l�F�c��MP
	int getMP() { return mMagicPoint; }
	// ����Ԃ�
	// �߂�l�F���I�u�W�F�N�g
	Fire& getFire() { return mFire; }
	// �U�����̃G���e�B�e�B��Ԃ�
	// �߂�l�F�U���̓����蔻��p�̃G���e�B�e�B
	BraveAttackCollision& getBraveAttackCollision() { return mBraveAttackCollision; }

	//==========================================================
	// setter
	//==========================================================
	
	// MP���Z�b�g����֐�
	// �����Fmp	�c��MP
	void setMP(const int mp) { mMagicPoint = mp; }
};
#endif
