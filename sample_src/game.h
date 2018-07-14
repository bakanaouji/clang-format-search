//==========================================================
/// @file
/// @brief    Game�N���X
/// @author   �������V
///
/// @attention  �Q�[���G���W���̊�ՂƂȂ�N���X�ł��B
///				�Q�[���̃��C���ƂȂ�N���X�͂��̃N���X���p������K�v������܂��B

//==========================================================
#ifndef _GAME_H                 // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _GAME_H                 // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

class Game;

#include <windows.h>
#include <Mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "entity.h"
#include "constants.h"
#include "textDX.h"
#include "console.h"
#include "gameError.h"
#include "messageDialog.h"
#include "inputDialog.h"

//==========================================================
// Game�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace gameNS
{
	const char FONT[] = "Courier New";  // �t�H���g
	const int POINT_SIZE = 14;          // �|�C���g�T�C�Y
	const COLOR_ARGB FONT_COLOR = SETCOLOR_ARGB(255, 255, 255, 255);    // ��
}

//==========================================================
// �Q�[���G���W���̊�ՂƂȂ�N���X�ł��B
// �Q�[���̃��C���ƂȂ�N���X�͂��̃N���X���p������K�v������܂��B
class Game
{
protected:
	// ���ʂ̃Q�[���v���p�e�B
	Graphics *mGraphics;            // Graphics�ւ̃|�C���^
	Input   *mInput;                // Input�ւ̃|�C���^
	Audio   *mAudio;                // Audio�ւ̃|�C���^
	Console *mConsole;              // Console�ւ̃|�C���^
	MessageDialog *mMessageDialog;  // MessageDialog�ւ̃|�C���^
	InputDialog *mInputDialog;      // pointer to InputDialog
	HWND    mHwnd;                  // �E�B���h�E�n���h��
	HRESULT mHr;                    // �W���̖߂�^
	LARGE_INTEGER mTimeStart;       // �p�t�H�[�}���X�J�E���^�[�̊J�n�l
	LARGE_INTEGER mTimeEnd;         // �p�t�H�[�}���X�J�E���^�[�̏I���l
	LARGE_INTEGER mTimerFreq;       // �p�t�H�[�}���X�J�E���^�[�̎��g��
	float   mFrameTime;             // �Ō�̃t���[���ɗv��������
	float   mFps;                   // �t���[�����[�g�i1�b������̃t���[�����j
	TextDX  mDxFont;                // �t���[�����[�g�p��DirectX�t�H���g
	bool    mFpsOn;                 // true�̏ꍇ�t���[�����[�g��\��
	DWORD   mSleepTime;             // �t���[���ԂŃX���[�v���鎞�ԁi�~���b�j
	bool    mPaused;                // �Q�[�����ꎞ��~����Ă���ꍇ�Atrue
	bool    mInitialized;			// �Q�[���̏������ɐ��������ꍇ�Atrue
	std::string  mCommand;          // �R���\�[���R�}���h

public:
	// �R���X�g���N�^
	Game();
	// �f�X�g���N�^
	virtual ~Game();

	// �����o�[�֐�

	// Windows���b�Z�[�W�n���h��
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// �Q�[����������
	// ���s�O�Fhwnd�̓E�B���h�E�ւ̃n���h��
	virtual void initialize(HWND hwnd);

	// WinMain���̃��C���̃��b�Z�[�W���[�v��run���J��Ԃ��Ăяo��
	virtual void run(HWND);

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
	// �\�񂳂�Ă����r�f�I�����������ׂĉ��
	virtual void releaseAll();

	// ���ׂẴT�[�t�F�C�X���č쐬���A���ׂẴG���e�B�e�B�����Z�b�g
	virtual void resetAll();

	// �\�񂳂�Ă��������������ׂč폜
	virtual void deleteAll();

	// �R���\�[���R�}���h������
	virtual void consoleCommand();

	// �Q�[���A�C�e���������_�[
	virtual void renderGame();

	// ���������O���t�B�b�N�X�f�o�C�X������
	virtual void handleLostGraphicsDevice();

	// �f�B�X�v���C���[�h���Z�b�g�i�t���X�N���[���A�E�B���h�E�������̓g�O���j
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	// Grpahics�ւ̃|�C���^��߂�
	Graphics* getGraphics() { return mGraphics; }

	// Input�ւ̃|�C���^��߂�
	Input* getInput() { return mInput; }

	// �Q�[�����I��
	void exitGame() { PostMessage(mHwnd, WM_DESTROY, 0, 0); }

	// Audio�ւ̃|�C���^��߂�
	Audio* getAudio() { return mAudio; }

	// �������z�֐��̐錾
	// �����̊֐��́AGame���p������N���X���ŋL�q����K�v������܂��B

	// �Q�[���A�C�e�����X�V
	virtual void update() = 0;

	// AI�v�Z�����s
	virtual void ai() = 0;

	// �Փ˂��`�F�b�N
	virtual void collisions() = 0;

	// �O���t�B�b�N�X�������_�[
	// graphics->spriteBegin();���Ăяo��
	// �X�v���C�g
	// grpahics->spriteEnd();���Ăяo��
	// �X�v���C�g�ȊO��`��
	virtual void render() = 0;
};

#endif
