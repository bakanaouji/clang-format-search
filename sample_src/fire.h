//==========================================================
/// @file
/// @brief    Fireクラス
/// @author   阿部拳之
///
/// @attention  プレイヤーの必殺技によって発射される炎を表すクラスです。

//==========================================================
#ifndef _FIRE_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _FIRE_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "character.h"
#include "constants.h"

//==========================================================

// Fireクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace fireNS
{
	const int WIDTH = 24;							// 画像の幅（各フレーム）
	const int HEIGHT = 32;							// 画像の高さ
	const int COLLISION_RADIUS = 4;					// 円の衝突判定
	const float SPEED = 300;						// 1秒あたりのピクセル数
	const float FIRE_DELAY = 0.5f;					// 炎発動の間隔は4秒
	const int TEXTURE_COLS = 12;					// テクスチャは8列
	const int START_FRAME = 52;						// アニメーションはフレーム52から開始
	const int END_FRAME = 52 + 36;					// アニメーションフレームは52、64、76、88
	const float ANIMATION_DELAY = 0.1f;				// フレーム間の時間
	const int FIRE_MP = 20;							// 必殺技に使用するMP
}

// プレイヤーの必殺技によって発射される炎を表すクラスです。
class Fire : public Entity							// Entityクラスを継承
{
private:
	float mFireTimer;								// 発射が可能になるまでの残り時間
public:
	// コンストラクタ
	Fire();

	//==========================================================
	// 継承されたメンバー関数
	//==========================================================

	// Update
	// エンティティがアクティブな場合、一定時間まっすぐ移動
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
	void update(const float frameTime);
	// パラメータリセット
	// roundStart()内で呼び出される
	void reset();

	// 新しいメンバー関数
	// 勇者から炎を発射
	// 指定した位置にこのエンティティをアクティブにして出現させる
	// 引数：centerX	攻撃を行ったエンティティの中心のX方向の位置
	// 引数：centerY	攻撃を行ったエンティティの中心のY方向の位置
	// 引数：width		攻撃を行ったエンティティの幅
	// 引数：height		攻撃を行ったエンティティの高さ
	// 引数：scale		攻撃を行ったエンティティのスケール
	// 引数：mp			攻撃を行ったエンティティの残りMP
	// 引数：direction	攻撃を行ったエンティティが向いている方向
	void fire(const float centerX, const float centerY, const float width, const float height, const float scale, int& mp, const characterNS::DIRECTION direction);
};
#endif
