//==========================================================
/// @file
/// @brief    Input�N���X
/// @author   �������V
///
/// @attention  ���[�U�[�̓��́i�}�E�X��L�[�{�[�h�j����������N���X�ł��B

//==========================================================
#ifndef _INPUT_H                // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA 
#define _INPUT_H                // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>
#include "constants.h"
#include "gameError.h"

// �����׃}�E�X�p
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif
//--------------------------

//==========================================================
// Input�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;     // �L�[�z��̒���

	// clear()�p�̒l�A�r�b�g�t���O
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const short GAMEPAD_THUMBSTICK_DEADZONE = (short)(0.20f * 0X7FFF);  // �f�b�h�]�[���Ƃ��Ĕ͈͂�20%���f�t�H���g�Ƃ���
const short GAMEPAD_TRIGGER_DEADZONE = 20;							// �g���K�[�͈̔͂�0����255�܂�
const DWORD MAX_CONTROLLERS = 4;									// XInput�ɂ���Ă��ځ[�Ƃ����R���g���[���[�̍ő吔

// �r�b�g��state.Gamepad.wButtons�̃Q�[���p�b�h�̃{�^���ɑΉ�
const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

struct ControllerState
{
	XINPUT_STATE        state;
	XINPUT_VIBRATION    vibration;
	float               vibrateTimeLeft;    // �~���b
	float               vibrateTimeRight;   // �~���b
	bool                connected;
};

// ���[�U�[�̓��́i�}�E�X��L�[�{�[�h�j����������N���X�ł��B
class Input
{
private:
	// �w��̃L�[��������Ă����Ԃ̏ꍇ��true
	bool mKeysDown[inputNS::KEYS_ARRAY_LEN];
	// �w��̃L�[�������ꂽ�ꍇ��true
	bool mKeysPressed[inputNS::KEYS_ARRAY_LEN];
	std::string mTextIn;                         // ���[�U�[�����͂����e�L�X�g
	char mCharIn;                                // �Ō�ɓ��͂��ꂽ����
	bool mNewLine;                               // �V�����s�̊J�n����true
	int  mMouseX, mMouseY;                       // �}�E�X�̉�ʍ��W
	int  mMouseRawX, mMouseRawY;                 // �����׃}�E�X�̃f�[�^
	int  mMouseWheel;                            // 
	RAWINPUTDEVICE mRid[1];                      // �����׃}�E�X�p
	bool mMouseCaptured;                         // �}�E�X���L���v�`������Ă���ꍇ��true
	bool mMouseLButton;                          // ���}�E�X�{�^����������Ă���ꍇ��true
	bool mMouseMButton;                          // �����}�E�X�{�^����������Ă���ꍇ��true
	bool mMouseRButton;                          // �E�}�E�X�{�^����������Ă���ꍇ��true
	bool mMouseX1Button;                         // X1�}�E�X�{�^����������Ă���ꍇ��true
	bool mMouseX2Button;                         // X2�}�E�X�{�^����������Ă���ꍇ��true
	ControllerState mControllers[MAX_CONTROLLERS];    // �R���g���[���[�̏��
	short mThumbstickDeadzone;
	short mTriggerDeadzone;

public:
	// �R���X�g���N�^
	Input();

	// �f�X�g���N�^
	virtual ~Input();

	// �}�E�X�ƃR���g���[���[�̓��͂�������
	// �}�E�X���L���v�`������ꍇ�Acapture=true��ݒ�
	// GameError���X���[
	// �����Fhwnd�@		�E�B���h�E�n���h��
	// �����Fcapture	�}�E�X���L���v�`�����邩�ǂ���
	void initialize(HWND hwnd, bool capture);

	// ���̃L�[�ɂ��āAkeysDown�z���keyPressed�z���true��ݒ�
	// ���s�O�FwParam�ɁA���z�L�[�R�[�h�i0~255�j���i�[����Ă���
	void keyDown(WPARAM);

	// ���̃L�[�ɂ��āAkeysDown�z���false��ݒ�
	// ���s�O�FwParam�ɁA���z�L�[�R�[�h�i0~255�j���i�[����Ă���
	void keyUp(WPARAM);

	// ���͂��ꂽ������textIn������ɕۑ�
	// ���s�O�FwParam�ɁA�������i�[����Ă���
	void keyIn(WPARAM);

	// �w�肳�ꂽ���z�L�[��������Ă���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
	bool isKeyDown(UCHAR vkey) const;

	// ���߃t���[���ɂ����āA�w��̉��z�L�[�������ꂽ���Ƃ�����ꍇ�Atrue��߂��܂��B
	// �L�[�̉��������̏�Ԃ́A�e�t���[���̏I�����ɏ�������܂��B
	bool wasKeyPressed(UCHAR vkey) const;

	// ���߂̃t���[���ɂ����āA���炩�̃L�[�������ꂽ�ꍇ�Atrue��߂��܂��B
	// �L�[�̉��������̏�Ԃ́A�e�t���[���̏I�����ɏ�������܂��B
	bool anyKeyPressed() const;

	// �w�肳�ꂽ�L�[�̉����������N���A
	void clearKeyPress(UCHAR vkey);

	// KEYS_DOWN�AKEYS_PRESSED�AMOUSE�ATEXT_IN��������KEYS_MOUSE_TEXT
	// �̔C�ӂ̑g�ݍ��킹�ł���w��̓��̓o�b�t�@���N���A
	// OR '|'�I�y���[�^�̎g�p�̓p�����[�^��g�ݍ��킹���Ӗ�����
	void clear(UCHAR what);

	// �L�[�A�}�E�X�A�e�L�X�g���̓o�b�t�@���N���A
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }

	// �e�L�X�g���̓o�b�t�@���N���A
	void clearTextIn() { mTextIn.clear(); }

	// �Ō�ɓ��͂��ꂽ�������N���A
	void clearCharIn() { mCharIn = 0; }

	// string�Ƃ��ē��͂��ꂽ�e�L�X�g��Ԃ�
	std::string getTextIn() { return mTextIn; }

	// �e�L�X�g��string�Ƃ��ē���
	void setTextIn(std::string str) { mTextIn = str; }

	// �Ō�ɓ��͂��ꂽ������Ԃ�
	char getCharIn() { return mCharIn; }

	// �}�E�X�̉�ʈʒu��ǂݎ��AmouseX��mouseY�ɕۑ�
	void mouseIn(LPARAM);

	// �}�E�X����̃��[�f�[�^��ǂݎ��AmouseRawX��mouseRawY�ɕۑ�
	// ���̃��[�`���́A�����׃}�E�X�ɑΉ����Ă��܂��B
	void mouseRawIn(LPARAM);

	// WHEEL_DELTA�i120�j�̔{���ŕ\�������}�E�X�z�C�[���̓�����ǂݎ��B
	// ���̒l�̓z�C�[�������[�U�[���痣�������ɉ�]���ꂽ���Ƃ������A
	// ���̒l�̓z�C�[�������[�U�[�Ɍ������ĉ�]���ꂽ���Ƃ������B
	void mouseWheelIn(WPARAM);

	// ���}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseLButton(bool b) { mMouseLButton = b; }

	// �����}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseMButton(bool b) { mMouseMButton = b; }

	// �E�}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseRButton(bool b) { mMouseRButton = b; }

	// X1�AX2�}�E�X�{�^���̏�Ԃ�ۑ�
	void setMouseXButton(WPARAM wParam) {
		mMouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mMouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}

	// �}�E�X��X�ʒu��߂�
	int  getMouseX()        const { return mMouseX; }

	// �}�E�X��Y�ʒu��߂�
	int  getMouseY()        const { return mMouseY; }

	// �}�E�X��X�ʒu�̈ړ��̃��[�f�[�^��߂��B���ւ̈ړ���<0�A�E�ւ̈ړ���>0
	// �����׃}�E�X�ɑΉ�
	int  getMouseRawX()
	{
		int rawX = mMouseRawX;
		mMouseRawX = 0;
		return rawX;
	}

	// �}�E�X��Y�ʒu�̈ړ��̃��[�f�[�^��߂��B��ւ̈ړ���<0�A���ւ̈ړ���>0
	// �����׃}�E�X�ɑΉ�
	int  getMouseRawY()
	{
		int rawY = mMouseRawY;
		mMouseRawY = 0;
		return rawY;
	}

	// �O�̈ʒu����̃}�E�X�z�C�[���̑��ΓI�ȓ�����Ԃ��B
	// ���̒l�̓z�C�[�������[�U�[���痣�������ɉ�]���ꂽ���Ƃ������A
	// ���̒l�̓z�C�[�������[�U�[�Ɍ������ĉ�]���ꂽ���Ƃ������B
	// �ړ��ʂ�WHEEL_DELTA�i120�j�̔{���ŕ\�������B
	int  getMouseWheel()
	{
		int wheel = mMouseWheel;
		mMouseWheel = 0;
		return wheel;
	}

	// ���}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseLButton()  const { return mMouseLButton; }

	// �����}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseMButton()  const { return mMouseMButton; }

	// �E�}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseRButton()  const { return mMouseRButton; }

	// X1�}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseX1Button() const { return mMouseX1Button; }

	// X2�}�E�X�{�^���̏�Ԃ�߂�
	bool getMouseX2Button() const { return mMouseX2Button; }

	// �ڑ����ꂽ�R���g���[���[�̏�Ԃ��`�F�b�N���X�V
	void checkControllers();

	// �ڑ�����Ă���R���g���[���[�̏�Ԃ�ǂݎ��
	void readControllers();

	// �X�e�B�b�N�̃f�b�h�]�[�����Z�b�g
	void setThumbstickDeadzone(short dz) { mThumbstickDeadzone = abs(dz); }

	// �g���K�[�̃f�b�h�]�[�����Z�b�g
	void setTriggerDeadzone(BYTE dz) { mTriggerDeadzone = dz; }

	// �X�e�B�b�N�̃f�b�h�]�[����Ԃ�
	short getThumbstickDeadzone() { return mThumbstickDeadzone; }

	// �g���K�[�̃f�b�h�]�[����Ԃ�
	BYTE getTriggerDeadzone() { return static_cast<BYTE>(mTriggerDeadzone); }

	// �w��̃Q�[���R���g���[���[�̏�Ԃ�߂�
	const ControllerState* getControllerState(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &mControllers[n];
	}

	// �w��̃R���g���[���[���ڑ�����Ă��邩�ǂ�����Ԃ�
	bool getGamepadConnected(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].connected;
	}

	// �R���g���[���[n�̃{�^���̏�Ԃ�߂�
	const WORD getGamepadButtons(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.wButtons;
	}

	// �R���g���[���[n�̕����p�b�h��̏�Ԃ�߂�
	bool getGamepadDPadUp(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
	}

	// �R���g���[���[n�̕����p�b�h���̏�Ԃ�߂�
	bool getGamepadDPadDown(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
	}

	// �R���g���[���[n�̕����p�b�h���̏�Ԃ�߂�
	bool getGamepadDPadLeft(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
	}

	// �R���g���[���[n�̕����p�b�h�E�̏�Ԃ�߂�
	bool getGamepadDPadRight(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
	}

	// �R���g���[���[n��START�{�^���̏�Ԃ�߂�
	bool getGamepadStart(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
	}

	// �R���g���[���[n��BACK�{�^���̏�Ԃ�߂�
	bool getGamepadBack(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
	}

	// �R���g���[���[n�̍��T���{�^���̏�Ԃ�߂�
	bool getGamepadLeftThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
	}

	// �R���g���[���[n�̉E�T���{�^���̏�Ԃ�߂�
	bool getGamepadRightThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
	}

	// �R���g���[���[n�̍��V�����_�[�{�^���̏�Ԃ�߂�
	bool getGamepadLeftShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
	}

	// �R���g���[���[n�̉E�V�����_�[�{�^���̏�Ԃ�߂�
	bool getGamepadRightShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	// �R���g���[���[n��A�{�^���̏�Ԃ�߂�
	bool getGamepadA(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
	}

	// �R���g���[���[n��B�{�^���̏�Ԃ�߂�
	bool getGamepadB(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
	}

	// �R���g���[���[n��X�{�^���̏�Ԃ�߂�
	bool getGamepadX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
	}

	// �R���g���[���[n��Y�{�^���̏�Ԃ�߂�
	bool getGamepadY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
	}

	// �R���g���[���[n�̍��g���K�[�̒l��Ԃ��i0����255�j�B
	BYTE getGamepadLeftTrigger(UINT n);

	// �R���g���[���[n�̍��g���K�[�̒l��Ԃ��i0����255�j�B
	// �f�b�h�]�[�����g�p���Ȃ��B
	BYTE getGamepadLeftTriggerUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.bLeftTrigger;
	}

	// �R���g���[���[n�̉E�g���K�[�̒l��Ԃ��i0����255�j�B
	BYTE getGamepadRightTrigger(UINT n);

	// �R���g���[���[n�̉E�g���K�[�̒l��Ԃ��i0����255�j�B
	// �f�b�h�]�[�����g�p���Ȃ��B
	BYTE getGamepadRightTriggerUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.bRightTrigger;
	}

	// �R���g���[���[n�̍��X�e�B�b�NX�̒l��Ԃ��i-32767����32767�j�B
	SHORT getGamepadThumbLX(UINT n);

	// �R���g���[���[n�̍��X�e�B�b�NX�̒l��Ԃ��i-32767����32767�j�B
	// �f�b�h�]�[�����g�p���Ȃ��B
	SHORT getGamepadThumbLXUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.sThumbLX;
	}

	// �R���g���[���[n�̍��X�e�B�b�NY�̒l��Ԃ��i-32767����32767�j�B
	SHORT getGamepadThumbLY(UINT n);

	// �R���g���[���[n�̍��X�e�B�b�NY�̒l��Ԃ��i-32767����32767�j�B
	// �f�b�h�]�[�����g�p���Ȃ��B
	SHORT getGamepadThumbLYUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.sThumbLY;
	}

	// �R���g���[���[n�̉E�X�e�B�b�NX�̒l��Ԃ��i-32767����32767�j�B
	SHORT getGamepadThumbRX(UINT n);

	// �R���g���[���[n�̉E�X�e�B�b�NX�̒l��Ԃ��i-32767����32767�j�B
	// �f�b�h�]�[�����g�p���Ȃ��B
	SHORT getGamepadThumbRXUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)   // if invalid controller number
			n = MAX_CONTROLLERS - 1;    // force valid
		return mControllers[n].state.Gamepad.sThumbRX;
	}

	// �R���g���[���[n�̉E�X�e�B�b�NY�̒l��Ԃ��i-32767����32767�j�B
	SHORT getGamepadThumbRY(UINT n);

	// �R���g���[���[n�̉E�X�e�B�b�NY�̒l��Ԃ��i-32767����32767�j�B
	// �f�b�h�]�[�����g�p���Ȃ��B
	SHORT getGamepadThumbRYUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.sThumbRY;
	}

	// �R���g���[���[n�̍����[�^�[��U��������
	// ���͒���g�U��
	// speed�F0=�I�t�A65536=100�p�[�Z���g
	// sec�F�U�������鎞�ԁi�b�j
	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		mControllers[n].vibration.wLeftMotorSpeed = speed;
		mControllers[n].vibrateTimeLeft = sec;
	}

	// �R���g���[���[n�̉E���[�^�[��U��������
	// ���͒���g�U��
	// speed�F0=�I�t�A65536=100�p�[�Z���g
	// sec�F�U�������鎞�ԁi�b�j
	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		mControllers[n].vibration.wRightMotorSpeed = speed;
		mControllers[n].vibrateTimeRight = sec;
	}

	// �ڑ�����Ă���R���g���[���[��U��������
	void vibrateControllers(float frameTime);
};

#endif

