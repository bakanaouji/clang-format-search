//==========================================================
/// @file
/// @brief    SystemGraphic�N���X
/// @author   �������V
///
/// @attention  �V�X�e���O���t�B�b�N�X���Ǘ�����N���X�ł��B

//==========================================================
#ifndef _SYSTEM_GRAPHIC_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _SYSTEM_GRAPHIC_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
//==========================================================

#include "text.h"
#include "textDX.h"
#include "image.h"
#include "braveIcon.h"
#include "castleIcon.h"
#include "hpTextImage.h"
#include "mpTextImage.h"
#include "dashboard.h"
#include "rect.h"

//==========================================================

// SystemGraphic�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace systemGraphicNS
{
	const int FONT_BIG_SIZE = 256;		// �t�H���g�̍���
	const int BRAVE_HEALTH_BAR_X = 100;	// �E�҂̗̑̓o�[�̈ʒuX
	const int BRAVE_HEALTH_BAR_Y = 0;	// �E�҂̗̑̓o�[�̈ʒuY
	const int BRAVE_MP_BAR_X = BRAVE_HEALTH_BAR_X;	// �E�҂�MP�o�[�̈ʒuX
	const int BRAVE_MP_BAR_Y = 25;		// �E�҂�MP�o�[�̈ʒuY
	const int CASTLE_HEALTH_BAR_X = 830;// ��̗̑̓o�[�̈ʒuX
	const int CASTLE_HEALTH_BAR_Y = 0;	// ��̗̑̓o�[�̈ʒuY
}

//==========================================================
// �V�X�e���O���t�B�b�N�X���Ǘ�����N���X�ł��B
class SystemGraphic
{
private:
	TextDX  mFontBig;									// �Q�[���o�i�[��DirectX�t�H���g
	Text*    mFontCK;									// �X�v���C�g�x�[�X�̃t�H���g
	Image mMenu;										// �^�C�g���摜
	Image mStageSelect;									// �X�e�[�W�I����ʂ̉摜
	Image mResult;                                      // ���U���g�摜
	Image mStageClear;									// �X�e�[�W�N���A�摜
	Image mDescription;                                 // ��������摜
	BraveIcon mBraveIcon;								// �E�҂̃A�C�R��
	CastleIcon mCastleIcon;								// ��̃A�C�R��
	BarGraph mBraveHealthBar;							// �E�҂̗̑̓o�[
	BarGraph mBraveMpBar;								// �E�҂�MP�o�[
	BarGraph mCastleHealthBar;							// ��̗̑̓o�[
	HpTextImage mBraveHpText;							// �E�҂̂g�o�e�L�X�g
	MpTextImage mBraveMpText;							// �E�҂̂l�o�e�L�X�g
	HpTextImage mCastleHpText;							// ��̂g�o�e�L�X�g
	Rect* mRect;										// Rect�ւ̃|�C���^�[
	bool mMenuOn;										// ���j���[�t���O
	bool mStageSelectOn;								// �X�e�[�W�I���t���O
	bool mDescriptionOn;                                // ��������t���O
	int mHighScores[3];									// �e�X�e�[�W�̃n�C�X�R�A
public:
	// �R���X�g���N�^
	SystemGraphic();

	// �f�X�g���N�^
	~SystemGraphic();

	//==========================================================
	// getter
	//==========================================================

	// �^�C�g���摜��Ԃ�
	// �߂�l�F�^�C�g���摜
	Image& getMenu() { return mMenu; }

	// �X�e�[�W�I���摜��Ԃ�
	// �߂�l�F�X�e�[�W�I���摜
	Image& getStageSelect() { return mStageSelect; }

	// ���U���g��ʂ�Ԃ�
	// �߂�l�F���U���g���
	Image& getResult() { return mResult; }

	// �X�e�[�W�N���A�摜��Ԃ�
	// �߂�l�F�X�e�[�W�N���A�摜
	Image& getStageClear() { return mStageClear; }

	// ��������摜��Ԃ�
	// �߂�l�F��������摜
	Image& getDescription() { return mDescription; }

	// �E�҂̃A�C�R����Ԃ�
	// �߂�l�F�E�҂̃A�C�R��
	BraveIcon& getBraveIcon() { return mBraveIcon; }

	// ��̃A�C�R����Ԃ�
	// �߂�l�F��̃A�C�R��
	CastleIcon& getCastleIcon() { return mCastleIcon; }

	// �E�҂̗̑̓o�[��Ԃ�
	// �߂�l�F�E�҂̗̑̓o�[
	BarGraph& getBraveHealthBar() { return mBraveHealthBar; }

	// �E�҂�MP�o�[��Ԃ�
	// �߂�l�F�E�҂�MP�o�[
	BarGraph& getBraveMpBar() { return mBraveMpBar; }

	// ��̗̑̓o�[��Ԃ�
	// �߂�l�F��̗̑̓o�[
	BarGraph& getCastleHealthBar() { return mCastleHealthBar; }

	// �E�҂�HP�e�L�X�g��Ԃ�
	// �߂�l�F�E�҂�HP�e�L�X�g
	HpTextImage& getBraveHpText() { return mBraveHpText; }

	// �E�҂�MP�e�L�X�g��Ԃ�
	// �߂�l�F�E�҂�MP�e�L�X�g
	MpTextImage& getBraveMpText() { return mBraveMpText; }

	// ���HP�e�L�X�g��Ԃ�
	// �߂�l�F���HP�e�L�X�g
	HpTextImage& getCastleHpText() { return mCastleHpText; }

	// Rect��Ԃ�
	// �߂�l�FRect
	Rect& getRect() { return *mRect; }

	// ���j���[��ʂ�\�������邩�ǂ�����Ԃ�
	// �߂�l�F���j���[��ʂ�\�������邩�ǂ���
	bool getMenuOn() { return mMenuOn; }

	// �X�e�[�W�I����ʂ�\�������邩�ǂ�����Ԃ�
	// �߂�l�F�X�e�[�W�I����ʂ�\�������邩�ǂ���
	bool getStageSelectOn() { return mStageSelectOn; }

	// ��������摜��\�������邩�ǂ�����Ԃ�
	// �߂�l�F��������摜��\�������邩�ǂ���
	bool getDescriptionOn() { return mDescriptionOn; }


	//==========================================================
	// setter
	//==========================================================

	// ���j���[��ʂ�\�������邩�ǂ������Z�b�g
	// �����F���j���[��ʂ�\�������邩�ǂ���
	void setMenuOn(bool menuOn) { mMenuOn = menuOn; }

	// �X�e�[�W�I����ʂ�\�������邩�ǂ������Z�b�g
	// �����F�X�e�[�W�I����ʂ�\�������邩�ǂ���
	void setStageSelectOn(bool stageSelectOn) { mStageSelectOn = stageSelectOn; }

	// ��������摜��\�������邩�ǂ������Z�b�g
	// �����F��������摜��\�������邩�ǂ���
	void setDescriptionOn(bool descriptionOn) { mDescriptionOn = descriptionOn; }


	// �v���C�̐V�������E���h���J�n
	void roundStart();
};
#endif // !_SYSTEM_GRAPHIC_H
