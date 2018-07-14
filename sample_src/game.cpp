//==========================================================
/// @file
/// @brief    game.hの実装
/// @author   阿部拳之
///
/// @attention  ゲームエンジンの基盤となるクラスです。
///				ゲームのメインとなるクラスはこのクラスを継承する必要があります。

//==========================================================

#include "game.h"

//==========================================================


//=============================================================================
// コンストラクタ
//=============================================================================
Game::Game()
{
	mInput = new Input();        // キーボード入力を即時に初期化
	// その他の初期化は、後で
	// input->initialize()を呼び出して処理
	mPaused = false;             // ゲームは一時停止中でない
	mGraphics = NULL;
	mAudio = NULL;
	mConsole = NULL;
	mMessageDialog = NULL;
	mInputDialog = NULL;
	mFps = 100;
	mFpsOn = false;              // デフォルトではフレームレートを表示しない
	mInitialized = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
Game::~Game()
{
	deleteAll();                // 予約されていたメモリをすべて解放
	ShowCursor(true);           // カーソルを表示
}

//=============================================================================
// Windowメッセージハンドラ
//=============================================================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (mInitialized)     // 初期化されていない場合はメッセージを処理しない
	{
		switch (msg)
		{
		case WM_DESTROY:
			// Windowsにこのプログラムを終了するように伝える
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:    // キーが押された
			mInput->keyDown(wParam);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:        // キーが離された
			mInput->keyUp(wParam);
			return 0;
		case WM_CHAR:                           // 文字が入力された
			mInput->keyIn(wParam);
			return 0;
		case WM_MOUSEMOVE:                      // マウスが移動された
			mInput->mouseIn(lParam);
			return 0;
		case WM_INPUT:                          // マウスからのローデータ入力
			mInput->mouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:                    // 左マウスボタンが押された
			mInput->setMouseLButton(true);
			mInput->mouseIn(lParam);            // マウスの位置
			return 0;
		case WM_LBUTTONUP:                      // 左マウスボタンが離された
			mInput->setMouseLButton(false);
			mInput->mouseIn(lParam);            // マウスの位置
			return 0;
		case WM_MBUTTONDOWN:                    // 中央マウスボタンが押された
			mInput->setMouseMButton(true);
			mInput->mouseIn(lParam);            // マウスの位置
			return 0;
		case WM_MBUTTONUP:                      // 中央マウスボタンが離された
			mInput->setMouseMButton(false);
			mInput->mouseIn(lParam);            // マウスの位置
			return 0;
		case WM_RBUTTONDOWN:                    // 右マウスボタンが押された
			mInput->setMouseRButton(true);
			mInput->mouseIn(lParam);            // マウスの位置
			return 0;
		case WM_RBUTTONUP:                      // 右マウスボタンが離された
			mInput->setMouseRButton(false);
			mInput->mouseIn(lParam);            // マウスの位置
			return 0;
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:	// マウスのXボタンが押された/離された
			mInput->setMouseXButton(wParam);
			mInput->mouseIn(lParam);            // マウスの位置
			return 0;
		case WM_MOUSEWHEEL:                     // マウスホイールが動いた
			mInput->mouseWheelIn(wParam);
			return 0;
		case WM_DEVICECHANGE:                   // コントローラーをチェック
			mInput->checkControllers();
			return 0;
		}
	}
	// Windowsに処理を任せる
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//=============================================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//=============================================================================
void Game::initialize(HWND hw)
{
	mHwnd = hw;									// ウィンドウハンドルを保存

	// グラフィックスを初期化
	mGraphics = new Graphics();
	// GameErrorをスロー
	mGraphics->initialize(mHwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// 入力を初期化、マウスをキャプチャしない
	mInput->initialize(mHwnd, false);           // GameErrorをスロー

	// コンソールを初期化
	mConsole = new Console();
	mConsole->initialize(mGraphics, mInput);    // コンソールを準備
	mConsole->print("---Console---");

	// messageDialogを初期化
	mMessageDialog = new MessageDialog();
	mMessageDialog->initialize(mGraphics, mInput, mHwnd);

	// initialize inputDialog
	mInputDialog = new InputDialog();
	mInputDialog->initialize(mGraphics, mInput, mHwnd);

	// DirectXフォントを初期化
	if (mDxFont.initialize(mGraphics, gameNS::POINT_SIZE, false, false, gameNS::FONT) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize DirectX font."));

	mDxFont.setFontColor(gameNS::FONT_COLOR);

	// サウンドシステムを初期化
	mAudio = new Audio();
	// サウンドファイルが定義されている場合
	if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0')
	{
		if (FAILED(mHr = mAudio->initialize()))
		{
			if (mHr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system because media file not found."));
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system."));
		}
	}

	// 高分解能タイマーの準備を試みる
	if (QueryPerformanceFrequency(&mTimerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

	QueryPerformanceCounter(&mTimeStart);        // 開始時間を取得

	mInitialized = true;
}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void Game::renderGame()
{
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];

	// レンダリングを開始
	if (SUCCEEDED(mGraphics->beginScene()))
	{
		// renderは、継承したクラス側で記述する必要のある純粋仮想関数です。
		render();	// 派生クラスのrenderを呼び出す

		mGraphics->spriteBegin();    // スプライトの描画を開始
		if (mFpsOn)					 // フレームレートの表示が要求されている場合
		{
			// fpsを文字列に変換
			_snprintf_s(buffer, BUF_SIZE, "fps %d ", (int)mFps);
			mDxFont.print(buffer, GAME_WIDTH - 100, GAME_HEIGHT - 28);
		}
		mGraphics->spriteEnd();      // スプライトの描画を終了

		mConsole->draw();			// コンソールは、ゲームの前面に表示されるようにここで描画
		mMessageDialog->draw();		// メッセージダイアログを前面に描画
		mInputDialog->draw();		// 入力用ダイアログをさらに前面に描画

		// レンダリングを終了
		mGraphics->endScene();
	}
	handleLostGraphicsDevice();

	// バックバッファを画面に表示
	mGraphics->showBackbuffer();
}

//=============================================================================
// 消失したグラフィックスデバイスを処理
//=============================================================================
void Game::handleLostGraphicsDevice()
{
	// デバイスの消失をテストし、それに応じて処理を実行
	mHr = mGraphics->getDeviceState();
	if (FAILED(mHr))					// グラフィックスデバイスが有効な状態でない場合
	{
		// デバイスが消失しており、リセットできる状態にない場合
		if (mHr == D3DERR_DEVICELOST)
		{
			Sleep(100);					// CPU時間を明け渡す（100ミリ秒）
			return;
		}
		// デバイスが消失しているが、リセットできる状態にある場合
		else if (mHr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			mHr = mGraphics->reset();	// グラフィックスデバイスのリセットを試みる
			if (FAILED(mHr))			// リセットが失敗した場合
				return;
			resetAll();
		}
		else
			return;						// 他のデバイスエラー
	}
}

//=============================================================================
// ディスプレイモードをセット（フルスクリーン、ウィンドウもしくはトグル）
//=============================================================================
void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	// 予約されていたビデオメモリをすべて解放
	releaseAll(); 
	mGraphics->changeDisplayMode(mode);
	// すべてのサーフェイスを再作成し、すべてのエンティティをリセット
	resetAll();
}

//=============================================================================
// WimMain内のメインのメッセージループで繰り返し呼び出される
//=============================================================================
void Game::run(HWND hwnd)
{
	if (mGraphics == NULL)            // グラフィックスが初期化されていない場合
		return;

	// 最後のフレームからの経過時間を計算、frameTimeに保存
	QueryPerformanceCounter(&mTimeEnd);
	mFrameTime = (float)(mTimeEnd.QuadPart - mTimeStart.QuadPart) / (float)mTimerFreq.QuadPart;

	// 省電力コード（winmm.libが必要）
	// 希望するフレームレートに対して経過時間が短い場合
	if (mFrameTime < MIN_FRAME_TIME)
	{
		mSleepTime = (DWORD)((MIN_FRAME_TIME - mFrameTime) * 1000);
		timeBeginPeriod(1);         // 1ミリ秒の分解能をWindowsタイマーに要求
		Sleep(mSleepTime);          // sleepTimeの間、CPUを解放
		timeEndPeriod(1);           // 1ミリ秒のタイマー分解能を終了
		return;
	}

	if (mFrameTime > 0.0)
		mFps = (mFps*0.99f) + (0.01f / mFrameTime);	// 平均fps
	if (mFrameTime > MAX_FRAME_TIME)				// フレームレートが非常に遅い場合
		mFrameTime = MAX_FRAME_TIME;				// 最大frameTimeを制限
	mTimeStart = mTimeEnd;

	// update()、ai()、collisions()は純粋仮想関数です。
	// これらの関数は、Gameを継承しているクラス側で記述する必要があります。
	if (!mPaused)								// 一時停止中でない場合
	{
		update();								// すべてのゲームアイテムを更新
		ai();									// 人工知能
		collisions();							// 衝突を処理
		mInput->vibrateControllers(mFrameTime);	// コントローラーの振動を処理
	}
	renderGame();								// すべてのゲームアイテムを描画

	// コンソールキーをチェック
	if (mInput->getCharIn() == CONSOLE_KEY)
	{
		mInput->clearCharIn();       // 最後に入力された文字をクリア
		mConsole->showHide();
		// コンソールが表示されている間、ゲームを一時停止
		mPaused = mConsole->getVisible();
	}
	consoleCommand();               // ユーザーが入力したコンソールコマンドを処理

	mInput->readControllers();		// コントローターの状態を読み取る
	mMessageDialog->update();		// ボタンクリックをチェック
	mInputDialog->update();

	mAudio->run();                  // サウンドエンジンの周期的タスクを実行

	// Alt+Enterでフルスクリーンモードとウィンドウモードを切り替え
//	if (mInput->isKeyDown(ALT_KEY) && mInput->wasKeyPressed(ENTER_KEY))
//		setDisplayMode(graphicsNS::TOGGLE); // フルスクリーンモードとウィンドウモードを切り替え

	// Escキーでウィンドウモードに設定
//	if (mInput->isKeyDown(ESC_KEY))
//		setDisplayMode(graphicsNS::WINDOW); // ウィンドウモードに設定

	// ポーズキーが押された場合、一時停止
	if (mInput->wasKeyPressed(VK_PAUSE))
		mPaused = !mPaused;

	// 入力をクリア
	// すべてのキーチェックが行われた後これを呼び出す
	mInput->clear(inputNS::KEYS_PRESSED);
}

//=============================================================================
// コンソールコマンドを処理
// 新しいコンソールコマンドを追加する場合は、
// この関数を派生クラスでオーバーライドする
//=============================================================================
void Game::consoleCommand()
{
	mCommand = mConsole->getCommand();   // コンソールからのコマンドを取得
	if (mCommand == "")                  // コマンドがない場合
		return;

	if (mCommand == "help")              // 「help」コマンドの場合
	{
		mConsole->print("Console Commands:");
		mConsole->print("fps - toggle display of frames per second");
		return;
	}

	if (mCommand == "fps")
	{
		mFpsOn = !mFpsOn;                // フレームレートの表示を切り替える
		if (mFpsOn)
			mConsole->print("fps On");
		else
			mConsole->print("fps Off");
	}
}

//=============================================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
//=============================================================================
void Game::releaseAll()
{
	safeOnLostDevice(mInputDialog);
	safeOnLostDevice(mMessageDialog);
	safeOnLostDevice(mConsole);
	mDxFont.onLostDevice();
	return;
}

//=============================================================================
// すべてのサーフェイスを再作成し、すべてのエンティティをリセット
//=============================================================================
void Game::resetAll()
{
	mDxFont.onResetDevice();
	safeOnResetDevice(mConsole);
	safeOnResetDevice(mMessageDialog);
	safeOnResetDevice(mInputDialog);
	return;
}

//=============================================================================
// 予約されていたメモリをすべて削除
//=============================================================================
void Game::deleteAll()
{
	// すべてのグラフィックスアイテムについてonLostDevice()を呼び出す
	releaseAll();
	safeDelete(mAudio);
	safeDelete(mGraphics);
	safeDelete(mInput);
	safeDelete(mConsole);
	safeDelete(mMessageDialog);
	safeDelete(mInputDialog);
	mInitialized = false;
}