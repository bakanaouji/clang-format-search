//==========================================================
/// @file
/// @brief    systemGraphic.hの実装
/// @author   阿部拳之
///
/// @attention  システムグラフィックスを管理するクラスです。

//==========================================================

#include "systemGraphic.h"

//==========================================================
// コンストラクタ
//==========================================================
SystemGraphic::SystemGraphic()
{
	mFontCK = new Text();	// スプライトフォント
	mMenuOn = true;			// メニュー表示フラグ
	mStageSelectOn = false;	// ステージ選択画面表示フラグ
	mDescriptionOn = false;	// 操作説明画面表示フラグ
	mRect = NULL;			// システムグラフィックス用の四角形
}

//==========================================================
// デストラクタ
//==========================================================
SystemGraphic::~SystemGraphic()
{
	safeDelete(mRect);		// システムグラフィックス用の四角形のメモリを解放
	safeDelete(mFontCK);		// スプライトフォントのメモリを解放
}

//==========================================================
// プレイの新しいラウンドを開始
//==========================================================
void SystemGraphic::roundStart()
{
	// リザルト画面の位置をリセット
	mResult.setY((float)-mResult.getHeight());
	// ステージクリア画面の位置をリセット
	mStageClear.setY((float)GAME_HEIGHT);
	// 勇者のアイコンの初期化
	mBraveIcon.reset();
}

//==========================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//==========================================================
void SystemGraphic::initialize(HWND hwnd)
{
	// システムグラフィックス用の四角形を生成、初期化
	mRect = new Rect();

	// スプライトテキストを初期化
	if (!mFontCK->initialize(mGraphics, FONT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));


}