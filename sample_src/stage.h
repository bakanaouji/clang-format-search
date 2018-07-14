//==========================================================
/// @file
/// @brief    Stage�N���X
/// @author   �������V
///
/// @attention  �X�e�[�W�����Ǘ�����N���X�ł��B
///				�E�ҁA�G�A�}�b�v���A�o���P�[�h�A��ȂǃX�e�[�W�Ɋւ�����͂����ŊǗ����܂��B

//==========================================================
#ifndef _STAGE_H	// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _STAGE_H	// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "enemy.h"
#include "midBoss.h"
#include "brave.h"
#include "castle.h"
#include "textureManager.h"

//==========================================================
// �X�e�[�W�����Ǘ�����N���X�ł��B
// �E�ҁA�G�A�}�b�v���A�o���P�[�h�A��ȂǃX�e�[�W�Ɋւ�����͂����ŊǗ����܂��B
class Stage
{
private:
	Map mMap;									// �X�e�[�W�̃}�b�v���
	Barricade mBarricades[mapNS::BARRICADE_NUM];// �o���P�[�h�̔z��
	Brave mBrave;								// �v���C���[
	Castle mCastle;								// ��
	Enemy **mEnemy;	    						// �G�ւ̃|�C���^�̔z��
	float *mEnemyX;								// �G�̏����ʒuX�̔z��
	float *mEnemyY;								// �G�̏����ʒuY�̔z��
	float mRemainingTime;						// �Q�[�����̎c�莞��
	int mEnemyNum;								// �G�̐�
	int mStageNum;								// �I�����Ă���X�e�[�W�̔ԍ�
	int mEnemyWaveNum;							// ���݂̓G�̔g�̔ԍ��i���g�A���g�A�A�A�j�A���g�����ׂē|��������g���A�A�Ɛi��
	bool    mRoundOver;							// ���E���h���I�������ꍇ�Atrue
	bool mClearedStage;							// �X�e�[�W���N���A�����ꍇ�Atrue
	bool mInitializedEnemies;					// �G���������ł�����true
public:
	// �R���X�g���N�^
	Stage();
	// �f�X�g���N�^
	~Stage();

	//==========================================================
	// getter
	//==========================================================

	// �}�b�v����Ԃ�
	// �߂�l�F�}�b�v���
	Map& getMap() { return mMap; }
	// �G�̐���Ԃ�
	// �߂�l�F�G�̐�
	int getEnemyNum() { return mEnemyNum; }
	// �X�e�[�W�ԍ���Ԃ�
	// �߂�l�F�X�e�[�W�ԍ�
	int getStageNum() { return mStageNum; }
	// �Q�[�����̎c�莞�Ԃ�Ԃ�
	// �߂�l�F�Q�[�����̎c�莞��
	float getRemainingTime() { return mRemainingTime; }
	// �w�肵���C���f�b�N�X�̓G��Ԃ�
	// �߂�l�F�w�肵���C���f�b�N�X�̓G
	Enemy& getEnemy(int index) { return *mEnemy[index]; }
	// �E�҂�Ԃ�
	// �߂�l�F�E��
	Brave& getBrave() { return mBrave; }
	// �w�肵���C���f�b�N�X�̃o���P�[�h��Ԃ�
	// �߂�l�F�w�肵���C���f�b�N�X�̃o���P�[�h
	Barricade& getBarricade(int index) { return mBarricades[index]; }
	// ���Ԃ�
	// �߂�l�F��
	Castle& getCastle() { return mCastle; }
	// �Q�[���I�[�o�[���ǂ�����Ԃ�
	// �߂�l�F�Q�[���I�[�o�[���ǂ���
	bool getRoundOver() { return mRoundOver; }
	// �X�e�[�W���N���A�������ǂ�����Ԃ�
	// �߂�l�F�X�e�[�W���N���A�������ǂ���
	bool getClearedStage() { return mClearedStage; }
	// �G���������ł��Ă��邩�ǂ���
	// �߂�l�F�G���������ł��Ă��邩�ǂ���
	bool getInitializedEnemies() { return mInitializedEnemies; }


	//==========================================================
	// setter
	//==========================================================
	// �X�e�[�W���N���A�������ǂ������Z�b�g
	// �����Fcs	�X�e�[�W���N���A�������ǂ���
	void setClearedStage(bool cs) { mClearedStage = cs; }
	// �X�e�[�W�ԍ����Z�b�g
	// �����Fsn	�X�e�[�W�ԍ�
	void setStageNum(int sn) { mStageNum = sn; }
	// �G���������ł��Ă��邩�ǂ������Z�b�g
	// �����FinitializedE	�G���������ł��Ă��邩�ǂ���
	void setInitializedEnemies(bool initializedE) { mInitializedEnemies = initializedE; }
	// �Q�[���I�[�o�[���ǂ������Z�b�g
	// �����FroundOver�Q�[���I�[�o�[���ǂ���
	void setRoundOver(bool roundOver) { mRoundOver = roundOver; }

	// �X�e�[�W����������
	void reset();
	// ���݂̃X�e�[�W�ԍ��ŃQ�[���X�^�[�g
	void roundStart();
	// �l�H�m�\
	// �e�G�̐l�H�m�\���v�Z
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void ai(float frameTime);
	// �Փ˂�����
	// ��������Փ˂͈ȉ��̒ʂ�F
	// �E�҂̍U���Ɗe�G�Ƃ̏Փ�
	// �e�G�̍U���ƗE�҂��A�o���P�[�h�Ƃ̏Փ�
	// �E�҂̕K�E�Z�i���j�Ɗe�G�̏Փ�
	// �e�G�ƗE�҂̏Փ�
	// �E�҂̍U���ƃo���P�[�h�Ƃ̏Փ�
	// �e�G�Ƃق��̃G���e�B�e�B�Ƃ̏Փˏ󋵂ɂ���āA�G�̏�Ԃ𕪊�
	// �@�U�����͂��̂܂܍U��
	// �A�E�҂ƏՓ˂��Ă���ꍇ�͗E�҂ɑ΂��Ă̍U����ԂɑJ��
	// �B��ƏՓ˂��Ă���ꍇ�͏�ɑ΂��Ă̍U����ԂɑJ��
	// �C�ŋߐڂ̃o���P�[�h�ɏՓ˂��Ă���ꍇ�̓o���P�[�h�ɑ΂��Ă̍U����ԂɑJ��
	// �D�����ꂩ�̃}�b�v�^�C���̒����܂ňړ������ꍇ�A
	//   �ŋߐڂ̃o���P�[�h�Ɍ������Ĉړ���ԂɑJ�ځi1/3���炢�̊m���ŕ����������_���Ɍ��߂�悤�ɂ��Ă���j
	void collisions();
	// stage��1�X�e�b�v���i�߂�
	// �����FframeTime�@1�t���[���Ōo�߂�������
	void runStage(float frameTime);
	// �G�̐����`�F�b�N���A�}�b�v��ɓG�����Ȃ���ΐV���ɐ���
	void checkCurrentEnemyNum();
	// �w�肳�ꂽ�X�e�[�W�̓G�f�[�^��ǂݍ���
	// �����FstageNum		�X�e�[�W�ԍ�
	// �����FenemyWave		�扽�h�ڂ̓G�̌Q��
	bool readEnemyFile(int stageNum, int enemyWave);
	// �w�肳�ꂽ�X�e�[�W�A�h�̓G�f�[�^��ǂݍ��ݓG������������
	// �����FstageNum		�X�e�[�W�ԍ�
	// �����FenemyWave		�扽�h�ڂ̓G�̌Q��
	void initializeEnemies(int stageNum, int enemyWave);
};
#endif