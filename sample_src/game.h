//==========================================================
/// @file
/// @brief    Gameクラス
/// @author   阿部拳之
///
/// @attention  ゲームエンジンの基盤となるクラスです。
///				ゲームのメインとなるクラスはこのクラスを継承する必要があります。

//==========================================================
#ifndef _GAME_H                 // このファイルが複数の箇所でインクルードされる場合に、
#define _GAME_H                 // 多重に定義されることを防ぎます。
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
// Gameクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace gameNS
{
	const char FONT[] = "Courier New";  // フォント
	const int POINT_SIZE = 14;          // ポイントサイズ
	const COLOR_ARGB FONT_COLOR = SETCOLOR_ARGB(255, 255, 255, 255);    // 白
}

//==========================================================
// ゲームエンジンの基盤となるクラスです。
// ゲームのメインとなるクラスはこのクラスを継承する必要があります。
class Game
{
protected:
	// 共通のゲームプロパティ
	Graphics *mGraphics;            // Graphicsへのポインタ
	Input   *mInput;                // Inputへのポインタ
	Audio   *mAudio;                // Audioへのポインタ
	Console *mConsole;              // Consoleへのポインタ
	MessageDialog *mMessageDialog;  // MessageDialogへのポインタ
	InputDialog *mInputDialog;      // pointer to InputDialog
	HWND    mHwnd;                  // ウィンドウハンドル
	HRESULT mHr;                    // 標準の戻り型
	LARGE_INTEGER mTimeStart;       // パフォーマンスカウンターの開始値
	LARGE_INTEGER mTimeEnd;         // パフォーマンスカウンターの終了値
	LARGE_INTEGER mTimerFreq;       // パフォーマンスカウンターの周波数
	float   mFrameTime;             // 最後のフレームに要した時間
	float   mFps;                   // フレームレート（1秒あたりのフレーム数）
	TextDX  mDxFont;                // フレームレート用のDirectXフォント
	bool    mFpsOn;                 // trueの場合フレームレートを表示
	DWORD   mSleepTime;             // フレーム間でスリープする時間（ミリ秒）
	bool    mPaused;                // ゲームが一時停止されている場合、true
	bool    mInitialized;			// ゲームの初期化に成功した場合、true
	std::string  mCommand;          // コンソールコマンド

public:
	// コンストラクタ
	Game();
	// デストラクタ
	virtual ~Game();

	// メンバー関数

	// Windowsメッセージハンドラ
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ゲームを初期化
	// 実行前：hwndはウィンドウへのハンドル
	virtual void initialize(HWND hwnd);

	// WinMain内のメインのメッセージループでrunを繰り返し呼び出す
	virtual void run(HWND);

	// グラフィックスデバイスが消失したときに呼び出す
	// グラフィックスデバイスをリセット可能にするため、
	// 予約されていたビデオメモリをすべて解放
	virtual void releaseAll();

	// すべてのサーフェイスを再作成し、すべてのエンティティをリセット
	virtual void resetAll();

	// 予約されていたメモリをすべて削除
	virtual void deleteAll();

	// コンソールコマンドを処理
	virtual void consoleCommand();

	// ゲームアイテムをレンダー
	virtual void renderGame();

	// 消失したグラフィックスデバイスを処理
	virtual void handleLostGraphicsDevice();

	// ディスプレイモードをセット（フルスクリーン、ウィンドウもしくはトグル）
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	// Grpahicsへのポインタを戻す
	Graphics* getGraphics() { return mGraphics; }

	// Inputへのポインタを戻す
	Input* getInput() { return mInput; }

	// ゲームを終了
	void exitGame() { PostMessage(mHwnd, WM_DESTROY, 0, 0); }

	// Audioへのポインタを戻す
	Audio* getAudio() { return mAudio; }

	// 純粋仮想関数の宣言
	// これらの関数は、Gameを継承するクラス内で記述する必要があります。

	// ゲームアイテムを更新
	virtual void update() = 0;

	// AI計算を実行
	virtual void ai() = 0;

	// 衝突をチェック
	virtual void collisions() = 0;

	// グラフィックスをレンダー
	// graphics->spriteBegin();を呼び出す
	// スプライト
	// grpahics->spriteEnd();を呼び出す
	// スプライト以外を描画
	virtual void render() = 0;
};

#endif
