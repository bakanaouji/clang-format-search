//==========================================================
/// @file
/// @brief    systemGraphic.h�̎���
/// @author   �������V
///
/// @attention  �V�X�e���O���t�B�b�N�X���Ǘ�����N���X�ł��B

//==========================================================

#include "systemGraphic.h"

//==========================================================
// �R���X�g���N�^
//==========================================================
SystemGraphic::SystemGraphic()
{
	mFontCK = new Text();	// �X�v���C�g�t�H���g
	mMenuOn = true;			// ���j���[�\���t���O
	mStageSelectOn = false;	// �X�e�[�W�I����ʕ\���t���O
	mDescriptionOn = false;	// ���������ʕ\���t���O
	mRect = NULL;			// �V�X�e���O���t�B�b�N�X�p�̎l�p�`
}

//==========================================================
// �f�X�g���N�^
//==========================================================
SystemGraphic::~SystemGraphic()
{
	safeDelete(mRect);		// �V�X�e���O���t�B�b�N�X�p�̎l�p�`�̃����������
	safeDelete(mFontCK);		// �X�v���C�g�t�H���g�̃����������
}

//==========================================================
// �v���C�̐V�������E���h���J�n
//==========================================================
void SystemGraphic::roundStart()
{
	// ���U���g��ʂ̈ʒu�����Z�b�g
	mResult.setY((float)-mResult.getHeight());
	// �X�e�[�W�N���A��ʂ̈ʒu�����Z�b�g
	mStageClear.setY((float)GAME_HEIGHT);
	// �E�҂̃A�C�R���̏�����
	mBraveIcon.reset();
}

//==========================================================
// �Q�[����������
// �G���[����GameError���X���[
//==========================================================
void SystemGraphic::initialize(HWND hwnd)
{
	// �V�X�e���O���t�B�b�N�X�p�̎l�p�`�𐶐��A������
	mRect = new Rect();

	// �X�v���C�g�e�L�X�g��������
	if (!mFontCK->initialize(mGraphics, FONT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));


}