//==========================================================
/// @file
/// @brief    定数ヘッダー
/// @author   阿部拳之
///
/// @attention  ゲーム内のパラメータや、画像・音声ファイル名などを定数として持っています。
///				いくつかのマクロ処理についても記述されています。

//==========================================================
#ifndef _CONSTANTS_H            // このファイルが複数の箇所でインクルードされる場合に、
#define _CONSTANTS_H            // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <windows.h>

//=============================================================================
//                  定数
//=============================================================================

// ウィンドウに関する定数
const char CLASS_NAME[] = "CastleDiffender";
const char GAME_TITLE[] = "CastleDiffender ver1.0";
const bool FULLSCREEN = false;							// フルスクリーンの場合true
const UINT GAME_WIDTH = 1280;							// ゲーム画面の幅（ピクセル単位）
const UINT GAME_HEIGHT = 720;							// ゲーム画面の高さ（ピクセル単位）

// ゲーム内の定数
const double PI = 3.14159265;							// 円周率
const float FRAME_RATE = 60.0f;							// フレームレート (frames/sec)
const float MIN_FRAME_RATE = 10.0f;						// 最小のフレームレート
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;			// 1フレームに要する最小の時間
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;		// 1フレームに要する最大の時間

// ゲーム内のパラメータ
// 勇者関係
const int BRAVE_ATTACK_DAMAGE = 50;					// 勇者の攻撃力（ダメージ量）
const int BRAVE_FIRE_DAMAGE = 110;					// 勇者のファイアー（必殺技）の攻撃力（ダメージ量）
const int BRAVE_RECOVERY_TO_BARRICADE = 20;			// 勇者の攻撃でのバリケードの回復量
const int BRAVE_MP_RECOVERY = 10;					// 勇者のMP回復量（mpRecoveryTime秒ごとに回復）
const float BRAVE_MP_RECOVERY_TIME = 2.0f;			// 勇者のMPが回復する間隔（秒）
const float BRAVE_MOVE_SPEED = 150.0f;				// 勇者の移動速度（ピクセル）
// 雑魚敵関係
const float ENEMY_ATTACK_DAMAGE = 2.5f;					// 雑魚敵の攻撃力（プレイヤーと城へのダメージ量）
const float ENEMY_ATTACK_DAMEGE_BARRICADE = 10.0f;		// 雑魚敵の攻撃力（バリケードへのダメージ量）
const float ENEMY_MOVE_SPEED = 80.0f;					// 移動速度（ピクセル）
// 中ボス関係
const float MIDBOSS_ATTACK_DAMAGE = 15.0f;				// 中ボスの攻撃力（プレイヤーと城へのダメージ量）
const float MIDBOSS_ATTACK_DAMEGE_BARRICADE = 20.0f;	// 雑魚敵の攻撃力（バリケードへのダメージ量）

// 画像
const char MENU_IMAGE[] = "pictures\\systemGraphics\\title.png";				// メニュー画像
const char STAGE_SELECT_IMAGE[] = "pictures\\systemGraphics\\stageselect.png";	// ステージ選択画面画像
const char RESULT_IMAGE[] = "pictures\\systemGraphics\\gameover.png";           // リザルト画像
const char STAGE_CLEAR_IMAGE[] = "pictures\\systemGraphics\\stageclear.png";	// ステージクリア画像
const char DESCRIPTION_IMAGE[] = "pictures\\systemGraphics\\description.png";   // 操作説明画像
const char BRAVE_MOVE_IMAGE[] = "pictures\\alex\\alex_main.png";				// 勇者の歩行画像
const char FIRE_IMAGE[] = "pictures\\alex\\fire.png";							// 炎の画像
const char ENEMY_IMAGE[] = "pictures\\enemy\\enemy.png";						// 雑魚敵の画像
const char MID_BOSS_IMAGE[] = "pictures\\enemy\\midBoss.png";					// 中ボスの画像
const char TILE_IMAGES[] = "pictures\\map\\ground.png";							// マップ画像
const char DASHBOARD_TEXTURES[] = "pictures\\systemGraphics\\dashboard.png";	// ダッシュボード画像
const char BARRICADE_IMAGE[] = "pictures\\map\\barricade.png";					// バリケードの画像
const char HIT_EFFECT_IMAGE[] = "pictures\\effect\\hitEffect.png";				// ダメージエフェクトの画像
const char ATTACK_EFFECT_IMAGE[] = "pictures\\effect\\attackEffect.png";		// 攻撃エフェクトの画像
const char COLLISION_IMAGE[] = "pictures\\alex\\attackCollision.png";			// 攻撃用のテクスチャ
const char CASTLE_IMAGE[] = "pictures\\map\\castle.png";						// 城画像
const char BRAVE_ICON_IMAGE[] = "pictures\\alex\\icon.png";						// 勇者のアイコン画像
const char TEXT_IMAGE[] = "pictures\\systemGraphics\\text.png";					// テキスト（ＭＰ、ＨＰ）画像
const char CASTLE_ICON_IMAGE[] = "pictures\\map\\castleIcon.png";				// 城のアイコンの画像
const char FONT_IMAGE[] = "pictures\\systemGraphics\\CKfont.png";				// スプライトテキストの画像

// 音声
// WAVE_BANK（xwbファイル）
const char WAVE_BANK[] = "audio\\Win\\Wave Bank.xwb";
// SOUND_BANK（xsbファイル）
const char SOUND_BANK[] = "audio\\Win\\Sound Bank.xsb";
// オーディオキュー
const char TITLE[]    = "title";	// タイトル中
const char STAGE[]    = "stage";	// ステージ中
const char CLEAR[]    = "clear";	// クリア時
const char GAMEOVER[] = "gameover";	// ゲームオーバー時
const char KILL[]     = "kill";		// プレイヤー攻撃時
const char FIRE_SE[]  = "fire";		// プレイヤー必殺技時


// キーマップ
const UCHAR CONSOLE_KEY = '`';				// `キー
const UCHAR ESC_KEY = VK_ESCAPE;			// Escキー
const UCHAR ALT_KEY = VK_MENU;				// Altキー
const UCHAR ENTER_KEY = VK_RETURN;			// Enterキー
const UCHAR BRAVE_LEFT_KEY = VK_LEFT;		// 左移動キー
const UCHAR BRAVE_RIGHT_KEY = VK_RIGHT;		// 右移動キー
const UCHAR BRAVE_UP_KEY = VK_UP;			// 上移動キー
const UCHAR BRAVE_DOWN_KEY = VK_DOWN;		// 下移動キー
const UCHAR BRAVE_ATTACK_KEY = 'A';			// 攻撃キー
const UCHAR BRAVE_GAURD_KEY = 'D';			// ガードキー
const UCHAR BRAVE_FIRE_KEY = 'S';			// 必殺技キー

// WEAPONの種類（炎、勇者の攻撃、勇者の第二撃、雑魚敵の攻撃、中ボスの攻撃）
enum WEAPON { FIRE, BRAVE_ATTACK, BRAVE_SECOND_ATTACK, ENEMY_ATTACK , MIDBOSS_ATTACK};

//=============================================================================
// マクロ処理
//=============================================================================
// ポインタ参照されるアイテムを安全に解放
template <typename T>
inline void safeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease            // 下位互換性を維持するため

// ポインタ参照されるアイテムを安全に削除
template <typename T>
inline void safeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE safeDelete              // 下位互換性を維持するため

// ポインタ参照される配列を安全に削除
template <typename T>
inline void safeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // 下位互換性を維持するため

// onLostDeviceを安全に呼び出し
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
	if (ptr)
		ptr->onLostDevice();
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // 下位互換性を維持するため

// onResetDeviceを安全に呼び出し
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
	if (ptr)
		ptr->onResetDevice();
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // 下位互換性を維持するため

#endif
