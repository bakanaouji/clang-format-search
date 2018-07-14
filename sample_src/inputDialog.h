//==========================================================
/// @file
/// @brief    InputDialog�N���X
/// @author   �������V
///
/// @attention  ���[�U�[�̓��͗p�̃_�C�A���O�{�b�N�X��\���N���X�ł��B

//==========================================================
#ifndef _INPUTDIALOG_H          // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA 
#define _INPUTDIALOG_H          // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"
#include "messageDialog.h"

//==========================================================

// InputDailog�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace inputDialogNS
{
    const COLOR_ARGB TEXT_BACK_COLOR = graphicsNS::WHITE;   // ���̓e�L�X�g�̔w�i
    const COLOR_ARGB TEXT_COLOR = graphicsNS::BLACK;        // ���̓e�L�X�g�̐F
}

//==========================================================
// ���b�Z�[�W�_�C�A���O���p��������̓_�C�A���O
// ���[�U�[�̓��͗p�̃_�C�A���O�{�b�N�X��\���N���X�ł��B
class InputDialog : public MessageDialog
{
private:
    std::string mInText;                 // ���̓e�L�X�g
    RECT        mInTextRect;
    RECT        mTempRect;
    COLOR_ARGB  mTextBackColor;          // �e�L�X�g�̈�̔w�i�̐F
    COLOR_ARGB  mTextFontColor;          // �e�L�X�g�̈�̃t�H���g�̐F
    LP_VERTEXBUFFER mInTextVerts;        // �e�L�X�g�̈�̒��_�o�b�t�@

public:
	// �R���X�g���N�^
    InputDialog();
	// �f�X�g���N�^
    virtual ~InputDialog();

	// ���_�o�b�t�@������
    void prepareVerts();

	// InputDialog��\��
    const void draw();

	// ���̓e�L�X�g��߂�
    std::string getText()   
    {
        if(!mVisible)
            return mInText;
        else
            return "";
    }

	// ���̓e�L�X�g�̃t�H���g�̐F��ݒ�
    void setTextFontColor(COLOR_ARGB fc)  {mTextFontColor = fc;}

	// ���̓e�L�X�g�̔w�i�̐F��ݒ�
    void setTextBackColor(COLOR_ARGB bc)  {mTextBackColor = bc;}

	// �e�L�X�gstr��InputDialog�ɕ\��
    void print(const std::string &str);

	// �u����v�C�x���g���`�F�b�N
    void update();

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
    void onLostDevice();
};

#endif

