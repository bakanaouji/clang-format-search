//==========================================================
/// @file
/// @brief    Enemy�N���X
/// @author   �������V
///
/// @attention  �G���G��\���N���X�ł��B
///				�o���P�[�h��j�󂵂Ȃ����Ɍ������čU�߂Ă���̂ŁA
///				�v���C���[�͗E�҂𑀍삵�ē|���܂��B

//==========================================================
#ifndef _ENEMY_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _ENEMY_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "character.h"
#include "constants.h"
#include "map.h"
#include "barricade.h"
#include "enemyAttackCollision.h"

//==========================================================

// Enemy�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace enemyNS
{
	const int WIDTH = 32;							// �摜�̕��i�e�t���[���j
	const int HEIGHT = 32;							// �摜�̍���
	const int TEXTURE_COLS = 12;					// �e�N�X�`����12��
	const int MOVE_UP_START_FRAME = 54;				// ������ړ��̃A�j���[�V�����̓t���[��54����J�n
	const int MOVE_UP_END_FRAME = 56;				// ������ړ��̃A�j���[�V�����t���[����54�A55�A56
	const int MOVE_RIGHT_START_FRAME = 66;			// �E�����ړ��̃A�j���[�V�����̓t���[��66����J�n
	const int MOVE_RIGHT_END_FRAME = 68;			// �E�����ړ��̃A�j���[�V�����t���[����66�A67�A68
	const int MOVE_DOWN_START_FRAME = 78;			// �������ړ��̃A�j���[�V�����̓t���[��78����J�n
	const int MOVE_DOWN_END_FRAME = 80;				// �������ړ��̃A�j���[�V�����t���[����78�A79�A80
	const int MOVE_LEFT_START_FRAME = 90;			// �������ړ��̃A�j���[�V�����̓t���[��90����J�n
	const int MOVE_LEFT_END_FRAME = 92;				// �������ړ��̃A�j���[�V�����t���[����90�A91�A92
	const float MOVE_ANIMATION_DELAY = 0.1f;		// �ړ��A�j���[�V�����̃t���[���Ԃ̎���
	const int ATTACK_UP_START_FRAME = 151;			// ������U���̃A�j���[�V�����̓t���[��151����J�n
	const int ATTACK_UP_END_FRAME = 151 + 36;		// ������U���ɃA�j���[�V�����t���[����151�A163�A175�A184
	const int ATTACK_RIGHT_START_FRAME = 148;		// �E�����U���̃A�j���[�V�����̓t���[��148����J�n
	const int ATTACK_RIGHT_END_FRAME = 148 + 36;	// �E�����U���̃A�j���[�V�����t���[����148�A161�A173�A182
	const int ATTACK_DOWN_START_FRAME = 103;		// �������U���̃A�j���[�V�����̓t���[��103����J�n
	const int ATTACK_DOWN_END_FRAME = 103 + 36;		// �������U���̃A�j���[�V�����t���[����103�A115�A127�A139
	const int ATTACK_LEFT_START_FRAME = 100;		// �������U���̃A�j���[�V�����̓t���[��100����J�n
	const int ATTACK_LEFT_END_FRAME = 100 + 36;		// �������U���̃A�j���[�V�����t���[����100�A112�A124�A136
	const float ATTACK_TIME = 0.8f;					// �v���C���[�ɗאڂ��Ă���U���Ɉڂ�܂ł̗��ߎ���
	const float CONTINUE_ATTACK_TIME = 0.8f;		// �A���ōU������ꍇ�̗��ߎ���
	const float DAMAGE_TIME = 1.0f;					// �_���[�W���󂯂Ă���ꍇ�ADAMAGE_TIME���Ƃɉ摜���_��
	enum TYPE { NORMAL, RED, BLUE };                // �G�̎��
}

// �G���G��\���N���X�ł��B
// �o���P�[�h��j�󂵂Ȃ����Ɍ������čU�߂Ă���̂ŁA
// �v���C���[�͗E�҂𑀍삵�ē|���܂��B
class Enemy : public Character
{
protected:
	Barricade* mBarricades;					// �o���P�[�h�̔z��ւ̃|�C���^
	EnemyCollision mAttackCollision;		// �U������p�̃R���W����
	characterNS::DIRECTION mOldDirection;	// �U�����O�Ɍ����Ă����������i�[
	characterNS::DIRECTION mGoalDirection;	// ���ɐi�ނׂ�����
	bool mIsAttacked;						// �U���������ォ�ǂ���
	bool mCanMakeDecesionMove;				// �ړ��Ɋւ��Ă̈ӎv������s���邩�ǂ���
	int mNearBarricadeIndex;				// �ŋߐڂ̃o���P�[�h�̃C���f�b�N�X
	float mAttackTimer;						// �U���A�j���[�V�����p�̃^�C�}�[
	float mContinueAttackTime;				// �A���ōU������ꍇ�̗��ߎ���
	float mAttackTime;						// �v���C���[�ɗאڂ��Ă���U���Ɉڂ�܂ł̗��ߎ���
	imageNS::MODE mAttackAnimationMode;		// �U�����̃A�j���[�V�����̑J�ڃ��[�h�i���������������j
	enemyNS::TYPE mType;                    // �G�̎��
public:
	// �R���X�g���N�^
	Enemy();

	//==========================================================
	// �p�����ꂽ�����o�[�֐�
	//==========================================================
	
	// Update
	// �U����������̏�ԑJ�ڂ�ړ��Ȃǂ̍s�����s���B
	// �S�̓I�ȏ�ԑJ�ڂɂ��ẮA�Փ˔���ɂ���čs������stage.collisions()�ɋL��
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	virtual void update(const float frameTime);
	// �l�H�m�\�BNPC�̍s�������肷��̂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
	// �����Fent		���̃G���e�B�e�B
	virtual void ai(const float frameTime, const Entity &ent) {};
	// �_���[�W����
	// WEAPON�̎�ނɂ���Ď󂯂�_���[�W������
	// �����Fweapon	WEAPON�̎��
	virtual void damage(const WEAPON weapon);
	// �p�����[�^���Z�b�g
	// roundStart()���ŌĂяo�����
	virtual void reset();

	//==========================================================
	// �V�����ǉ����郁���o�[�֐�
	//==========================================================

	//==========================================================
	// getter
	//==========================================================
	
	// ���ɐi�ނׂ�������Ԃ��֐�
	// �߂�l�F���ɐi�ނׂ�����
	characterNS::DIRECTION getGoalDirection() { return mGoalDirection; }
	// �ړ��Ɋւ��Ă̈ӎv������s���邩�ǂ���
	// �߂�l�F�ړ��Ɋւ��Ă̈ӎv������s���邩�ǂ���
	bool canMakeDecesionMoving() { return mCanMakeDecesionMove; }
	// �ŋߐڂ̃o���P�[�h�̃C���f�b�N�X��Ԃ�
	// �߂�l�F�ŋߐڂ̃o���P�[�h�̃C���f�b�N�X
	int getNearBarricadeIndex() { return mNearBarricadeIndex; }
	// �U������p�̃R���W������Ԃ�
	// �߂�l�F�U���̓����蔻��p�̃G���e�B�e�B
	EnemyCollision& getAttackCollision() { return mAttackCollision; }
	// �G�̎�ނ��擾����֐�
	// �߂�l�F�G�̎��
	enemyNS::TYPE getEnemyType() { return mType; }
	

	//==========================================================
	// setter
	//==========================================================
	
	// �o���P�[�h�̔z��ւ̃|�C���^���Z�b�g����֐�
	// �����F*barricades	�o���P�[�h�̔z��ւ̃|�C���^
	void setBarricadesPointer(Barricade *barricades) { mBarricades = barricades; }
	// ���ɐi�ނׂ��������Z�b�g����֐�
	// �����FgoalDirection	���ɐi�ނׂ�����
	void setGoalDirection(const characterNS::DIRECTION goalDirection) { mGoalDirection = goalDirection; }
	// �G�̎�ނ��Z�b�g����֐�
	// �����Ftype	�G�̎��
	void setEnemyType(const enemyNS::TYPE type){mType = type;}

	// ������goalDirection�ւƃA�b�v�f�[�g����֐�
	// �����FstrF	goalDirection�̌����̃A�j���[�V�����̍ŏ��̃t���[��
	// �����FendF	goalDirection�̌����̃A�j���[�V�����̍Ō�̃t���[��
	void changeDirection(const int strF, const int endF);
	// ��ԋ߂��̃o���P�[�h�̃C���f�b�N�X��Ԃ��֐�
	int searchNearBarricadeIndex();
	// �G���G�����܂ł̒�����Ƀo���P�[�h�����݂�����true�A���݂��Ȃ��ꍇ��false��Ԃ��B
	bool checkBarricadeOnLine();
	// �U�����[�h�Ƀ`�F���W����Ƃ��ɌĂяo���֐�
	// �����FcollisionVector	�Փ˂��Ă���G���e�B�e�B�ւ̃x�N�g��
	virtual void changeAttack(VECTOR2 &collisionVector);
	// �U�����[�h�Ƀ`�F���W����Ƃ��ɌĂяo���֐�
	// �����Fdir	�Փ˂��Ă���G���e�B�e�B���������
	virtual void changeAttack(const characterNS::DIRECTION dir);
	// �^�C���̒����ɃL�����N�^�[�����邩�ǂ���
	virtual bool isCenterOfTile();
	// �w�肵�������ɐi�߂邩�ǂ���
	// �����Fdir	�i�߂邩�m�F����������
	bool canMoveTo(const characterNS::DIRECTION dir);
};
#endif
