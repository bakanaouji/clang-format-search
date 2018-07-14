//==========================================================
/// @file
/// @brief    Map�N���X
/// @author   �������V
///
/// @attention  �X�e�[�W���̃}�b�v����ێ�����N���X�ł��B

//==========================================================
#ifndef _MAP_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _MAP_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <sstream> //�����X�g���[��
#include "gameError.h"

using namespace std;

//==========================================================
// Map�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace mapNS
{
	const int TEXTURE_SIZE = 32;		// �摜�T�C�Y�i1�^�C�����j
	const int TEXTURE_COLS = 16;		// �s��
	const int MAP_HEIGHT = 23;			// �}�b�v�̍���
	const int MAP_WIDTH = 40;			// �}�b�v�̕�
	const int BARRICADE_NUM = 8;		// �����o���P�[�h�̐�
	const int MAP_ROCK = 9;				// �}�b�v�f�[�^�ł́A��̃}�b�v�`�b�v�̊��蓖�āi�v���O�������ł͎g�p���Ȃ����A���Y�^�Ƃ��āj
	const int COL_ROCK = 1;				// �����蔻��f�[�^�ł́A��̊��蓖��
	const int COL_CASTLE = 2;			// �����蔻��f�[�^�ł́A��̊��蓖��
	const int OBJ_BARRICADE = 0;		// �I�u�W�F�N�g�f�[�^�ł́A�o���P�[�h�̊��蓖��
	const int OBJ_NEAR_BARRICADE = -2;	// �I�u�W�F�N�g�f�[�^�ł́A�o���P�[�h�ɗאڂ��Ă���^�C���̊��蓖��
	const int OBJ_NEAR_CASTLE = 1;		// �I�u�W�F�N�g�f�[�^�ł́A��ɗאڂ��Ă���^�C���̊��蓖��
	const int OBJ_NONE = -1;			// �I�u�W�F�N�g�f�[�^�ł́A�����Ȃ��^�C���̊��蓖��
	const int COL_ENEMY_INCLEMENT = 3;	// �����蔻��f�[�^�ł́A�G�̊��蓖�āi���̒l�����̒l�ɑ����j
}

//==========================================================
// �X�e�[�W���̃}�b�v����ێ�����N���X�ł��B
// �^�C�����W��(��ʏ�̍��W / 32)�ŗ^�����܂��B
class Map : public Entity
{
private:
	// �}�b�v�f�[�^
	// �ǂ̃^�C���ɂǂ̃}�b�v�`�b�v��`�悷�邩�����߂�f�[�^
	// ��̎�����͂����9�ɐݒ肵�A����ȊO�͎��R�Ƀ}�b�v�`�b�v��I��ł悢
	int mMapData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	// �����蔻��̃f�[�^
	// �Q�[����ʂ��Ĉړ��ł��Ȃ��ӏ���1��2�ɐݒ肷��A���1�ɁA���2�ɂ���
	// �G������^�C���ɂ͌��̒l+3��ݒ肷��A���̒l�͓G�̃^�C�����Ȃ�ׂ����Ԃ�Ȃ��悤�ɂ��邱�Ƃɗ��p
	int mColData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	// �����蔻��̃f�[�^�̏����l
	// �����l���Ƃ邱�Ƃœ����蔻��f�[�^�����Z�b�g���₷������
	int mColDataInit[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	// �}�b�v��̃I�u�W�F�N�g�f�[�^
	// �����Ȃ��I�u�W�F�N�g�i�o���P�[�h���ɗאڂ��Ă���ƓG�ɒm�点�邽�߂̃I�u�W�F�N�g�j���Ǘ�����
	// �o���P�[�h��0�A�o���P�[�h�ɗאڂ��Ă���^�C���̈ꕔ�ɂ�-2���A��ɗאڂ��Ă���^�C���ɂ�1�����蓖�Ă�
	// ���̃f�[�^�͓G�̈ӎv����ɂ݂̂��悤���邽�߁A�v���C���[�͗��p���Ȃ�
	int mObjData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
public:
	// �R���X�g���N�^
	Map();

	//==========================================================
	// �p�����ꂽ�����o�֐�
	//==========================================================
	
	// �}�b�v��`��
	virtual void draw();

	//==========================================================
	// �V�����ǉ����郁���o�֐�
	//==========================================================

	//==========================================================
	// getter
	//==========================================================

	// �}�b�v�f�[�^�̎擾�@
	// �����Fx	�擾�������^�C�����WX
	// �����Fy	�擾�������^�C�����WY
	int getMapData(const int x, const int y)
	{
		return mMapData[x][y];
	}
	// �}�b�v�����蔻��f�[�^�̎擾
	// �����Fx	�擾�������^�C�����WX
	// �����Fy	�擾�������^�C�����WY
	int getMapCol(const int x, const int y)
	{
		return mColData[x][y];
	}
	// �}�b�v��̃I�u�W�F�N�g�f�[�^�X�V
	// �����Fx	�擾�������^�C�����WX
	// �����Fy	�擾�������^�C�����WY
	int getMapObj(const int x, const int y)
	{
		return mObjData[x][y];
	}

	// �}�b�v�f�[�^�̍X�V
	// �����Fx		�X�V���������WX
	// �����Fy		�X�V���������WY
	// �����Fval	�}�b�v�f�[�^
	void updateMapData(const float x, const float y, const int val);
	// �}�b�v�����蔻��̍X�V �����͍X�V���������W�l�ƍX�V����l
	// val -> 0 : ����Ȃ��@val -> 1 : ���肠��
	// �����Fx		�X�V���������WX
	// �����Fy		�X�V���������WY
	// �����Fval	�����蔻��̃f�[�^
	void updateMapCol(const float x, const float y, const int val);
	// �}�b�v��̃I�u�W�F�N�g�f�[�^�X�V
	// �����Fx		�X�V���������WX
	// �����Fy		�X�V���������WY
	// �����Fval	�I�u�W�F�N�g�̃f�[�^
	void updateMapObj(const float x, const float y, const int val);
	// �}�b�v��̃I�u�W�F�N�g�f�[�^�X�V
	// �����Fx		�X�V�������^�C�����WX
	// �����Fy		�X�V�������^�C�����WY
	// �����Fval	�}�b�v�f�[�^
	void updateMapObjInt(const int y, const int x, const int val);
	// �}�b�v��̓����蔻��f�[�^���ׂĂ����Z�b�g����֐�
	void resetMapCol() {
		for (int i = 0; i < mapNS::MAP_HEIGHT; ++i)
		{
			for (int j = 0; j < mapNS::MAP_WIDTH; ++j)
			{
				mColData[i][j] = mColDataInit[i][j];
			}
		}
	}
	// �w�肳�ꂽ�}�b�v�̓����蔻��f�[�^�����Z�b�g����
	// �����Fx		���Z�b�g�������^�C�����WX
	// �����Fy		���Z�b�g�������^�C�����WY
	void resetMapCol(const int y, const int x) {
		if (x < 0 || x >= mapNS::MAP_WIDTH || y < 0 || y >= mapNS::MAP_HEIGHT) { return; }
		mColData[y][x] = mColDataInit[y][x];
	}
	// �w�肳�ꂽ�X�e�[�W�̃}�b�v�f�[�^��ǂݍ���
	// �����FstageNum	�X�e�[�W�ԍ�
	void readMapFile(const int stageNum);
};

#endif