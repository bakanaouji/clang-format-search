//==========================================================
/// @file
/// @brief    SystemGraphicクラス
/// @author   阿部拳之
///
/// @attention  システムグラフィックスを管理するクラスです。

//==========================================================
#ifndef _SYSTEM_GRAPHIC_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _SYSTEM_GRAPHIC_H		// 多重に定義されることを防ぎます。
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

// SystemGraphicクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace systemGraphicNS
{
	const int FONT_BIG_SIZE = 256;		// フォントの高さ
	const int BRAVE_HEALTH_BAR_X = 100;	// 勇者の体力バーの位置X
	const int BRAVE_HEALTH_BAR_Y = 0;	// 勇者の体力バーの位置Y
	const int BRAVE_MP_BAR_X = BRAVE_HEALTH_BAR_X;	// 勇者のMPバーの位置X
	const int BRAVE_MP_BAR_Y = 25;		// 勇者のMPバーの位置Y
	const int CASTLE_HEALTH_BAR_X = 830;// 城の体力バーの位置X
	const int CASTLE_HEALTH_BAR_Y = 0;	// 城の体力バーの位置Y
}

//==========================================================
// システムグラフィックスを管理するクラスです。
class SystemGraphic
{
private:
	TextDX  mFontBig;									// ゲームバナーのDirectXフォント
	Text*    mFontCK;									// スプライトベースのフォント
	Image mMenu;										// タイトル画像
	Image mStageSelect;									// ステージ選択画面の画像
	Image mResult;                                      // リザルト画像
	Image mStageClear;									// ステージクリア画像
	Image mDescription;                                 // 操作説明画像
	BraveIcon mBraveIcon;								// 勇者のアイコン
	CastleIcon mCastleIcon;								// 城のアイコン
	BarGraph mBraveHealthBar;							// 勇者の体力バー
	BarGraph mBraveMpBar;								// 勇者のMPバー
	BarGraph mCastleHealthBar;							// 城の体力バー
	HpTextImage mBraveHpText;							// 勇者のＨＰテキスト
	MpTextImage mBraveMpText;							// 勇者のＭＰテキスト
	HpTextImage mCastleHpText;							// 城のＨＰテキスト
	Rect* mRect;										// Rectへのポインター
	bool mMenuOn;										// メニューフラグ
	bool mStageSelectOn;								// ステージ選択フラグ
	bool mDescriptionOn;                                // 操作説明フラグ
	int mHighScores[3];									// 各ステージのハイスコア
public:
	// コンストラクタ
	SystemGraphic();

	// デストラクタ
	~SystemGraphic();

	//==========================================================
	// getter
	//==========================================================

	// タイトル画像を返す
	// 戻り値：タイトル画像
	Image& getMenu() { return mMenu; }

	// ステージ選択画像を返す
	// 戻り値：ステージ選択画像
	Image& getStageSelect() { return mStageSelect; }

	// リザルト画面を返す
	// 戻り値：リザルト画面
	Image& getResult() { return mResult; }

	// ステージクリア画像を返す
	// 戻り値：ステージクリア画像
	Image& getStageClear() { return mStageClear; }

	// 操作説明画像を返す
	// 戻り値：操作説明画像
	Image& getDescription() { return mDescription; }

	// 勇者のアイコンを返す
	// 戻り値：勇者のアイコン
	BraveIcon& getBraveIcon() { return mBraveIcon; }

	// 城のアイコンを返す
	// 戻り値：城のアイコン
	CastleIcon& getCastleIcon() { return mCastleIcon; }

	// 勇者の体力バーを返す
	// 戻り値：勇者の体力バー
	BarGraph& getBraveHealthBar() { return mBraveHealthBar; }

	// 勇者のMPバーを返す
	// 戻り値：勇者のMPバー
	BarGraph& getBraveMpBar() { return mBraveMpBar; }

	// 城の体力バーを返す
	// 戻り値：城の体力バー
	BarGraph& getCastleHealthBar() { return mCastleHealthBar; }

	// 勇者のHPテキストを返す
	// 戻り値：勇者のHPテキスト
	HpTextImage& getBraveHpText() { return mBraveHpText; }

	// 勇者のMPテキストを返す
	// 戻り値：勇者のMPテキスト
	MpTextImage& getBraveMpText() { return mBraveMpText; }

	// 城のHPテキストを返す
	// 戻り値：城のHPテキスト
	HpTextImage& getCastleHpText() { return mCastleHpText; }

	// Rectを返す
	// 戻り値：Rect
	Rect& getRect() { return *mRect; }

	// メニュー画面を表示させるかどうかを返す
	// 戻り値：メニュー画面を表示させるかどうか
	bool getMenuOn() { return mMenuOn; }

	// ステージ選択画面を表示させるかどうかを返す
	// 戻り値：ステージ選択画面を表示させるかどうか
	bool getStageSelectOn() { return mStageSelectOn; }

	// 操作説明画像を表示させるかどうかを返す
	// 戻り値：操作説明画像を表示させるかどうか
	bool getDescriptionOn() { return mDescriptionOn; }


	//==========================================================
	// setter
	//==========================================================

	// メニュー画面を表示させるかどうかをセット
	// 引数：メニュー画面を表示させるかどうか
	void setMenuOn(bool menuOn) { mMenuOn = menuOn; }

	// ステージ選択画面を表示させるかどうかをセット
	// 引数：ステージ選択画面を表示させるかどうか
	void setStageSelectOn(bool stageSelectOn) { mStageSelectOn = stageSelectOn; }

	// 操作説明画像を表示させるかどうかをセット
	// 引数：操作説明画像を表示させるかどうか
	void setDescriptionOn(bool descriptionOn) { mDescriptionOn = descriptionOn; }


	// プレイの新しいラウンドを開始
	void roundStart();
};
#endif // !_SYSTEM_GRAPHIC_H
