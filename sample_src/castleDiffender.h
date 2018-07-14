//==========================================================
/// @file
/// @brief    CastleDiffenceクラス
/// @author   阿部拳之
///
/// @attention  ゲームのメインクラスです。
///				1フレームごとの更新や描画はこのクラスで行います。

//==========================================================
#ifndef _CASTLE_DIFFENDER_H	// このファイルが複数の箇所でインクルードされる場合に、
#define _CASTLE_DIFFENDER_H	// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "game.h"
#include "text.h"
#include "textureManager.h"
#include "image.h"
#include "brave.h"
#include "braveAttackCollision.h"
#include "enemyAttackCollision.h"
#include "fire.h"
#include "map.h"
#include "castle.h"
#include "enemy.h"
#include "midBoss.h"
#include "dashboard.h"
#include "rect.h"
#include "barricade.h"
#include "braveIcon.h"
#include "castleIcon.h"
#include "hpTextImage.h"
#include "mpTextImage.h"
#include "hitEffect.h"
#include "attackEffect.h"
#include <string>
#include "stage.h"

//==========================================================

// CastleDiffenderクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace castleDiffenderNS
{
	const int FONT_BIG_SIZE = 256;		// フォントの高さ
	const int BRAVE_HEALTH_BAR_X = 100;	// 勇者の体力バーの位置X
	const int BRAVE_HEALTH_BAR_Y = 0;	// 勇者の体力バーの位置Y
	const int BRAVE_MP_BAR_X = BRAVE_HEALTH_BAR_X;	// 勇者のMPバーの位置X
	const int BRAVE_MP_BAR_Y = 25;		// 勇者のMPバーの位置Y
	const int CASTLE_HEALTH_BAR_X = 830;// 城の体力バーの位置X
	const int CASTLE_HEALTH_BAR_Y = 0;	// 城の体力バーの位置Y
	const int ROUND_TIME = 3;           // 新しいラウンドが開始するまでの時間
}

//==========================================================
// Gameクラスを継承してCastleDiffenderクラスを作成
// ゲームのメインクラスです。
// 1フレームごとの更新や描画はこのクラスで行います。
class CastleDiffender : public Game
{
private:
	// ゲームアイテム
	Stage mStage;										// ステージ情報
	TextDX  mFontBig;									// ゲームバナーのDirectXフォン
	Text*    mFontCK;									// sprite based font
	TextureManager mMenuTexture;						// タイトルのテクスチャ
	TextureManager mStageSelectTexture;					// ステージ選択画面のテクスチャ
	TextureManager mResultTexture;                      // リザルトのテクスチャ
	TextureManager mStageClearTexture;					// ステージクリア画面のテクスチャ
	TextureManager mDescriptionTexture;                 // 操作説明のテクスチャ
	TextureManager mBraveTexture;						// 勇者のテクスチャ
	TextureManager mAttackEffectTexture;				// 攻撃の衝撃波のテクスチャ
	TextureManager mBraveIconTexture;					// 勇者のアイコンのテクスチャ
	TextureManager mAttackCollisionTexture;				// 勇者の攻撃の当たり判定用のテクスチャ
	TextureManager mTileTexture;						// タイルのテクスチャ
	TextureManager mBarricadeTexture;					// バリケードのテクスチャ
	TextureManager mHitEffectTexture;					// 攻撃がヒットしたときのアニメーション画像のテクスチャ
	TextureManager mCastleTexture;						// 城のテクスチャ
	TextureManager mCastleIconTexture;					// 城のアイコンのテクスチャ
	TextureManager mFireTexture;						// 炎のテクスチャ
	TextureManager mEnemyTexture;						// 雑魚敵のテクスチャ
	TextureManager mMidBossTexture;						// 中ボスのテクスチャ
	TextureManager mDashboardTextures;					// ダッシュボードテクスチャ
	TextureManager mTextTexture;						// テキスト（ＭＰ、ＨＰ）のテクスチャ
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
	float   mRoundTimer;								// 新しいラウンドが開始するまでの時間
	int mHighScores[3];									// 各ステージのハイスコア

	//==========================================================
	// 内部的にのみ使用する関数
	//==========================================================
	
	// メニュー画面時の描画
	void drawMenu();

	// ステージ選択画面時の描画
	void drawStageSelect();

	// 操作説明画面時の描画
	void drawDescription();

	// ステージ中の描画
	void drawOnStage();

	// ゲームオーバー時の描画
	void drawOnRoundOver();

	// ステージクリア時の描画
	void drawOnClearedStage();

public:
	// コンストラクタ
	CastleDiffender();

	// デストラクタ
	virtual ~CastleDiffender();

	// ゲームを初期化
	// エラー時にGameErrorをスロー
	void initialize(HWND hwnd);

	//==========================================================
	// Gameからの純粋仮想関数をオーバーライドする必要がある
	//==========================================================

	// すべてのゲームアイテムを更新
	void update();
	// 人工知能
	void ai();
	// 衝突を処理
	void collisions();
	// ゲームアイテムをレンダー
	// プレイ中のレイヤー：マップ→黒背景→その他アイテムの順
	void render();

	// プレイの新しいラウンドを開始
	void roundStart();

	// コンソールコマンドを処理
	// デバッグ用
	void consoleCommand();

	// グラフィックスデバイスが消失した場合
	// グラフィックスデバイスをリセット可能にするため、
	// 予約されていたビデオメモリをすべて解放
	void releaseAll();

	// グラフィックスデバイスがリセットされた場合
	// すべてのサーフェイスを再作成
	void resetAll();


	// ゲームオーバー時に呼び出す
	void gameOver();

	// ステージクリア時に呼び出す
	void clearStage();

	// 敵のテクスチャを初期化する
	void initializeEnemiesTexture();
	
	// 各ステージのハイスコアを読み込む
	void loadHighScore();

};
#endif
