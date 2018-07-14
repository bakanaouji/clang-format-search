//==========================================================
/// @file
/// @brief    Inputクラス
/// @author   阿部拳之
///
/// @attention  ユーザーの入力（マウスやキーボード）を処理するクラスです。

//==========================================================
#ifndef _INPUT_H                // このファイルが複数の箇所でインクルードされる場合に、 
#define _INPUT_H                // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>
#include "constants.h"
#include "gameError.h"

// 高精細マウス用
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif
//--------------------------

//==========================================================
// Inputクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;     // キー配列の長さ

	// clear()用の値、ビットフラグ
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const short GAMEPAD_THUMBSTICK_DEADZONE = (short)(0.20f * 0X7FFF);  // デッドゾーンとして範囲の20%をデフォルトとする
const short GAMEPAD_TRIGGER_DEADZONE = 20;							// トリガーの範囲は0から255まで
const DWORD MAX_CONTROLLERS = 4;									// XInputによってさぼーとされるコントローラーの最大数

// ビットはstate.Gamepad.wButtonsのゲームパッドのボタンに対応
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
	float               vibrateTimeLeft;    // ミリ秒
	float               vibrateTimeRight;   // ミリ秒
	bool                connected;
};

// ユーザーの入力（マウスやキーボード）を処理するクラスです。
class Input
{
private:
	// 指定のキーが押されている状態の場合にtrue
	bool mKeysDown[inputNS::KEYS_ARRAY_LEN];
	// 指定のキーが押された場合にtrue
	bool mKeysPressed[inputNS::KEYS_ARRAY_LEN];
	std::string mTextIn;                         // ユーザーが入力したテキスト
	char mCharIn;                                // 最後に入力された文字
	bool mNewLine;                               // 新しい行の開始時にtrue
	int  mMouseX, mMouseY;                       // マウスの画面座標
	int  mMouseRawX, mMouseRawY;                 // 高精細マウスのデータ
	int  mMouseWheel;                            // 
	RAWINPUTDEVICE mRid[1];                      // 高精細マウス用
	bool mMouseCaptured;                         // マウスがキャプチャされている場合にtrue
	bool mMouseLButton;                          // 左マウスボタンが押されている場合にtrue
	bool mMouseMButton;                          // 中央マウスボタンが押されている場合にtrue
	bool mMouseRButton;                          // 右マウスボタンが押されている場合にtrue
	bool mMouseX1Button;                         // X1マウスボタンが押されている場合にtrue
	bool mMouseX2Button;                         // X2マウスボタンが押されている場合にtrue
	ControllerState mControllers[MAX_CONTROLLERS];    // コントローラーの状態
	short mThumbstickDeadzone;
	short mTriggerDeadzone;

public:
	// コンストラクタ
	Input();

	// デストラクタ
	virtual ~Input();

	// マウスとコントローラーの入力を初期化
	// マウスをキャプチャする場合、capture=trueを設定
	// GameErrorをスロー
	// 引数：hwnd　		ウィンドウハンドル
	// 引数：capture	マウスをキャプチャするかどうか
	void initialize(HWND hwnd, bool capture);

	// このキーについて、keysDown配列とkeyPressed配列にtrueを設定
	// 実行前：wParamに、仮想キーコード（0~255）が格納されている
	void keyDown(WPARAM);

	// このキーについて、keysDown配列にfalseを設定
	// 実行前：wParamに、仮想キーコード（0~255）が格納されている
	void keyUp(WPARAM);

	// 入力された文字をtextIn文字列に保存
	// 実行前：wParamに、文字が格納されている
	void keyIn(WPARAM);

	// 指定された仮想キーが押されている場合はtrue、それ以外の場合はfalseを戻す
	bool isKeyDown(UCHAR vkey) const;

	// 直近フレームにおいて、指定の仮想キーが押されたことがある場合、trueを戻します。
	// キーの押し下げの状態は、各フレームの終了時に消去されます。
	bool wasKeyPressed(UCHAR vkey) const;

	// 直近のフレームにおいて、何らかのキーが押された場合、trueを戻します。
	// キーの押し下げの状態は、各フレームの終了時に消去されます。
	bool anyKeyPressed() const;

	// 指定されたキーの押し下げをクリア
	void clearKeyPress(UCHAR vkey);

	// KEYS_DOWN、KEYS_PRESSED、MOUSE、TEXT_INもしくはKEYS_MOUSE_TEXT
	// の任意の組み合わせである指定の入力バッファをクリア
	// OR '|'オペレータの使用はパラメータを組み合わせを意味する
	void clear(UCHAR what);

	// キー、マウス、テキスト入力バッファをクリア
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }

	// テキスト入力バッファをクリア
	void clearTextIn() { mTextIn.clear(); }

	// 最後に入力された文字をクリア
	void clearCharIn() { mCharIn = 0; }

	// stringとして入力されたテキストを返す
	std::string getTextIn() { return mTextIn; }

	// テキストをstringとして入力
	void setTextIn(std::string str) { mTextIn = str; }

	// 最後に入力された文字を返す
	char getCharIn() { return mCharIn; }

	// マウスの画面位置を読み取り、mouseXとmouseYに保存
	void mouseIn(LPARAM);

	// マウスからのローデータを読み取り、mouseRawXとmouseRawYに保存
	// このルーチンは、高精細マウスに対応しています。
	void mouseRawIn(LPARAM);

	// WHEEL_DELTA（120）の倍数で表現されるマウスホイールの動きを読み取る。
	// 正の値はホイールがユーザーから離れる方向に回転されたことを示し、
	// 負の値はホイールがユーザーに向かって回転されたことを示す。
	void mouseWheelIn(WPARAM);

	// 左マウスボタンの状態を保存
	void setMouseLButton(bool b) { mMouseLButton = b; }

	// 中央マウスボタンの状態を保存
	void setMouseMButton(bool b) { mMouseMButton = b; }

	// 右マウスボタンの状態を保存
	void setMouseRButton(bool b) { mMouseRButton = b; }

	// X1、X2マウスボタンの状態を保存
	void setMouseXButton(WPARAM wParam) {
		mMouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mMouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}

	// マウスのX位置を戻す
	int  getMouseX()        const { return mMouseX; }

	// マウスのY位置を戻す
	int  getMouseY()        const { return mMouseY; }

	// マウスのX位置の移動のローデータを戻す。左への移動は<0、右への移動は>0
	// 高精細マウスに対応
	int  getMouseRawX()
	{
		int rawX = mMouseRawX;
		mMouseRawX = 0;
		return rawX;
	}

	// マウスのY位置の移動のローデータを戻す。上への移動は<0、下への移動は>0
	// 高精細マウスに対応
	int  getMouseRawY()
	{
		int rawY = mMouseRawY;
		mMouseRawY = 0;
		return rawY;
	}

	// 前の位置からのマウスホイールの相対的な動きを返す。
	// 正の値はホイールがユーザーから離れる方向に回転されたことを示し、
	// 負の値はホイールがユーザーに向かって回転されたことを示す。
	// 移動量はWHEEL_DELTA（120）の倍数で表現される。
	int  getMouseWheel()
	{
		int wheel = mMouseWheel;
		mMouseWheel = 0;
		return wheel;
	}

	// 左マウスボタンの状態を戻す
	bool getMouseLButton()  const { return mMouseLButton; }

	// 中央マウスボタンの状態を戻す
	bool getMouseMButton()  const { return mMouseMButton; }

	// 右マウスボタンの状態を戻す
	bool getMouseRButton()  const { return mMouseRButton; }

	// X1マウスボタンの状態を戻す
	bool getMouseX1Button() const { return mMouseX1Button; }

	// X2マウスボタンの状態を戻す
	bool getMouseX2Button() const { return mMouseX2Button; }

	// 接続されたコントローラーの状態をチェックし更新
	void checkControllers();

	// 接続されているコントローラーの状態を読み取る
	void readControllers();

	// スティックのデッドゾーンをセット
	void setThumbstickDeadzone(short dz) { mThumbstickDeadzone = abs(dz); }

	// トリガーのデッドゾーンをセット
	void setTriggerDeadzone(BYTE dz) { mTriggerDeadzone = dz; }

	// スティックのデッドゾーンを返す
	short getThumbstickDeadzone() { return mThumbstickDeadzone; }

	// トリガーのデッドゾーンを返す
	BYTE getTriggerDeadzone() { return static_cast<BYTE>(mTriggerDeadzone); }

	// 指定のゲームコントローラーの状態を戻す
	const ControllerState* getControllerState(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &mControllers[n];
	}

	// 指定のコントローラーが接続されているかどうかを返す
	bool getGamepadConnected(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].connected;
	}

	// コントローラーnのボタンの状態を戻す
	const WORD getGamepadButtons(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.wButtons;
	}

	// コントローラーnの方向パッド上の状態を戻す
	bool getGamepadDPadUp(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
	}

	// コントローラーnの方向パッド下の状態を戻す
	bool getGamepadDPadDown(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
	}

	// コントローラーnの方向パッド左の状態を戻す
	bool getGamepadDPadLeft(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
	}

	// コントローラーnの方向パッド右の状態を戻す
	bool getGamepadDPadRight(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
	}

	// コントローラーnのSTARTボタンの状態を戻す
	bool getGamepadStart(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
	}

	// コントローラーnのBACKボタンの状態を戻す
	bool getGamepadBack(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
	}

	// コントローラーnの左サムボタンの状態を戻す
	bool getGamepadLeftThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
	}

	// コントローラーnの右サムボタンの状態を戻す
	bool getGamepadRightThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
	}

	// コントローラーnの左ショルダーボタンの状態を戻す
	bool getGamepadLeftShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
	}

	// コントローラーnの右ショルダーボタンの状態を戻す
	bool getGamepadRightShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	// コントローラーnのAボタンの状態を戻す
	bool getGamepadA(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
	}

	// コントローラーnのBボタンの状態を戻す
	bool getGamepadB(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
	}

	// コントローラーnのXボタンの状態を戻す
	bool getGamepadX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
	}

	// コントローラーnのYボタンの状態を戻す
	bool getGamepadY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((mControllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
	}

	// コントローラーnの左トリガーの値を返す（0から255）。
	BYTE getGamepadLeftTrigger(UINT n);

	// コントローラーnの左トリガーの値を返す（0から255）。
	// デッドゾーンを使用しない。
	BYTE getGamepadLeftTriggerUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.bLeftTrigger;
	}

	// コントローラーnの右トリガーの値を返す（0から255）。
	BYTE getGamepadRightTrigger(UINT n);

	// コントローラーnの右トリガーの値を返す（0から255）。
	// デッドゾーンを使用しない。
	BYTE getGamepadRightTriggerUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.bRightTrigger;
	}

	// コントローラーnの左スティックXの値を返す（-32767から32767）。
	SHORT getGamepadThumbLX(UINT n);

	// コントローラーnの左スティックXの値を返す（-32767から32767）。
	// デッドゾーンを使用しない。
	SHORT getGamepadThumbLXUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.sThumbLX;
	}

	// コントローラーnの左スティックYの値を返す（-32767から32767）。
	SHORT getGamepadThumbLY(UINT n);

	// コントローラーnの左スティックYの値を返す（-32767から32767）。
	// デッドゾーンを使用しない。
	SHORT getGamepadThumbLYUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.sThumbLY;
	}

	// コントローラーnの右スティックXの値を返す（-32767から32767）。
	SHORT getGamepadThumbRX(UINT n);

	// コントローラーnの右スティックXの値を返す（-32767から32767）。
	// デッドゾーンを使用しない。
	SHORT getGamepadThumbRXUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)   // if invalid controller number
			n = MAX_CONTROLLERS - 1;    // force valid
		return mControllers[n].state.Gamepad.sThumbRX;
	}

	// コントローラーnの右スティックYの値を返す（-32767から32767）。
	SHORT getGamepadThumbRY(UINT n);

	// コントローラーnの右スティックYの値を返す（-32767から32767）。
	// デッドゾーンを使用しない。
	SHORT getGamepadThumbRYUndead(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return mControllers[n].state.Gamepad.sThumbRY;
	}

	// コントローラーnの左モーターを振動させる
	// 左は低周波振動
	// speed：0=オフ、65536=100パーセント
	// sec：振動させる時間（秒）
	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		mControllers[n].vibration.wLeftMotorSpeed = speed;
		mControllers[n].vibrateTimeLeft = sec;
	}

	// コントローラーnの右モーターを振動させる
	// 左は低周波振動
	// speed：0=オフ、65536=100パーセント
	// sec：振動させる時間（秒）
	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		mControllers[n].vibration.wRightMotorSpeed = speed;
		mControllers[n].vibrateTimeRight = sec;
	}

	// 接続されているコントローラーを振動させる
	void vibrateControllers(float frameTime);
};

#endif

