//==========================================================
/// @file
/// @brief    Console�N���X
/// @author   �������V
///
/// @attention  �f�o�b�O�p�̃R���\�[����\���N���X�ł��B
///				�u`�v�ŕ\������A����̃R�}���h��ł��ƂŃf�o�b�O���\�ł��B

//==========================================================
#ifndef _CONSOLE_H              // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _CONSOLE_H              // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

//==========================================================

// Console�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace consoleNS
{
    const UINT WIDTH = 1000;										// �R���\�[���̋�`�̕��i�s�N�Z���P�ʁj
    const UINT HEIGHT = 400;										// �R���\�[���̋�`�̍����i�s�N�Z���P�ʁj
    const UINT X = 5;												// �R���\�[���̍��������̉�ʈʒu
    const UINT Y = 5;
    const UINT MARGIN = 4;											// �R���\�[���̒[����e�L�X�g�܂ł̗]��
    const char FONT[] = "Courier New";								// �R���\�[���̃e�L�X�g�Ɏg�p����t�H���g
    const int FONT_HEIGHT = 14;										// �R���\�[���̃t�H���g�̍���
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;				// �R���\�[���̃t�H���g�̐F
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192,128,128,128);   // �R���\�[���̔w�i�̐F
    const int MAX_LINES = 256;										// �e�L�X�g�o�b�t�@���̕\���\�ȃe�L�X�g�̍ő�s��
}

// �f�o�b�O�p�̃R���\�[����\���N���X�ł��B
//�u`�v�ŕ\������A����̃R�}���h��ł��ƂŃf�o�b�O���\�ł��B
class Console
{
private:
    Graphics    *mGraphics;              // �O���t�B�b�N�V�X�e���ւ̃|�C���^
    Input       *mInput;                 // ���̓V�X�e���ւ̃|�C���^
    TextDX      mDxFont;                 // DirectX�t�H���g
    float       mX,mY;                   // �R���\�[���̈ʒu
    int         mRows;                   // �R���\�[���Ɏ��܂�e�L�X�g�̍s��
    std::string mCommandStr;             // �R���\�[���R�}���h
    std::string mInputStr;               // �R���\�[���ւ̓��̓e�L�X�g
    std::deque<std::string> mText;       // �f�b�Ntext
    RECT        mTextRect;               // �e�L�X�g�\��rect
    COLOR_ARGB  mFontColor;              // �t�H���g�J���[(a,r,g,b)
    COLOR_ARGB  mBackColor;              // �w�i�F(a,r,g,b)
    VertexC mVtx[4];                     // �w�i�p��vertex�f�[�^
    LP_VERTEXBUFFER mVertexBuffer;       // vertex�f�[�^�ۑ��p�̃o�b�t�@
    int         mScrollAmount;           // �X�N���[������Ă���s��
    bool        mInitialized;            // �������ɐ��������ꍇ�Atrue
    bool        mVisible;                // �R���\�[����\��������ꍇ�Atrue

public:
	// �R���X�g���N�^
    Console();

	// �f�X�g���N�^
    virtual ~Console();

	// �R���\�[����������
	// �����Fg	�O���t�B�b�N�V�X�e���ւ̃|�C���^
	// �����Fin	���̓V�X�e���ւ̃|�C���^
    bool initialize(Graphics *g, Input *in);

	// �R���\�[����`��
	// ���s�O�FBeginScene��EndScene�̓����ɂ���
    const void draw();

	// �R���\�[���̕\��/��\���𐧌䂷��v���p�e�B�𔽓]�����܂��B
	// �\������Ă���R���\�[���͔�\���ɂȂ�A
	// ��\���̃R���\�[���͕\�������悤�ɂȂ�܂��B
    void showHide();

	// �\��/��\���𐧌䂷��v���p�e�B�̌��݂̏�Ԃ�߂��܂��B
	// �߂�l�F�\����Ԃ���\����Ԃ�
    bool getVisible() {return mVisible;}

	// �R���\�[����\�����܂��B
    void show() {mVisible = true;}

	// �R���\�[�����\���ɂ��܂��B
    void hide() {mVisible = false;}

	// �R���\�[���f�B�X�v���C�Ƀe�L�X�g�������ǉ����܂��B
	// �����Fstr	�R���\�[���f�B�X�v���C�ɕ\��������������
    void print(const std::string &str);

	// �V���O���L�[�̃R���\�[���R�}���h���������܂��B
	// ���[�U�[�����͂����R�}���h�������߂��܂��B
	// �߂�l�F���[�U�[�����͂����R�}���h
    std::string getCommand();

	// �R���\�[���̓��̓e�L�X�g��߂��܂�
	// �߂�l�F�R���\�[���̓��̓e�L�X�g
    std::string getInput() {return mInputStr;}

	// �R���\�[���̓��̓e�L�X�g���N���A���܂�
    void clearInput()   {mInputStr = "";}

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
    void onLostDevice();

	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
    void onResetDevice();
};

#endif

