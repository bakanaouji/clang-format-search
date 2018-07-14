//==========================================================
/// @file
/// @brief    AttackEffectクラス
/// @author   阿部拳之
///
/// @attention  攻撃時のアニメーションを表示するクラスです。

//==========================================================
#ifndef _ATTACK_EFFECT_H	// このファイルが複数の箇所でインクルードされる場合、
#define _ATTACK_EFFECT_H	// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "image.h"
#include "character.h"

//==========================================================

// AttackEffectクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace attackEffectNS
{
	const int WIDTH = 96;					// 画像の幅（各フレーム）
	const int HEIGHT = 96;					// 画像の高さ
	const int TEXTURE_COLS = 5;				// テクスチャは5列
	const int START_FRAME = 0;				// アニメーションはフレーム0から開始
	const int END_FRAME = 3;				// アニメーションフレームは0、1、2、3
	const float ANIMATION_DELAY = 0.05f;	// アニメーションのフレーム間の時間
	const float SCALE = 0.8f;				// スプライトのスケール
}

//==========================================================
// 攻撃時アニメーションを表示するクラスです。
class AttackEffect : public Image
{
public:
	// コンストラクタ
	AttackEffect();


	//==========================================================
	// 継承されたメンバ関数
	//==========================================================
	
	// Update
	// アニメーションを更新する。
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
	void update (const float frameTime);

	//==========================================================
	// 新しく追加するメンバー関数
	//==========================================================

	// Attack
	// 攻撃の衝撃波のアニメーションを指定された位置に発動
	// 引数：centerX	攻撃を行ったエンティティの中心のX方向の位置
	// 引数：centerY	攻撃を行ったエンティティの中心のY方向の位置
	// 引数：width		攻撃を行ったエンティティの幅
	// 引数：height		攻撃を行ったエンティティの高さ
	// 引数：direction	攻撃を行ったエンティティが向いている方向
	void attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction);
};
#endif // !_ATTACK_EFFECT_H
