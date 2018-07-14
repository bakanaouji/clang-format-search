//==========================================================
/// @file
/// @brief    HitEffectクラス
/// @author   阿部拳之
///
/// @attention  攻撃がヒットしたときのアニメーションを表示するクラスです。

//==========================================================
#ifndef _HIT_EFFECT_H	// このファイルが複数の箇所でインクルードされる場合に、
#define _HIT_EFFECT_H	// 多重に定義されることを防ぎます。
//==========================================================

#include "image.h"
#include "constants.h"

// HitEffectクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace hitEffectNS
{
	const int WIDTH = 108;				// 画像の幅（各フレーム）
	const int HEIGHT = 108;				// 画像の高さ
	const int TEXTURE_COLS = 3;			// テクスチャは3列
	const int START_FRAME = 0;			// アニメーションはフレーム0から開始
	const int END_FRAME = 2;			// アニメーションフレームは0、1、2
	const float ANIMATION_DELAY = 0.1f;	// アニメーションのフレーム間の時間
}

// 攻撃がヒットしたときのアニメーションを表示するクラスです。
class HitEffect : public Image
{
public:
	// コンストラクタ
	HitEffect();

	//==========================================================
	// 継承されたメンバ関数
	//==========================================================

	// Update
	// アニメーションを更新する。
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
	void update(const float frameTime);

	// Hit
	// 攻撃ヒットのアニメーションを指定された座標の四方にランダムに表示
	// 引数：x	アニメーションの位置X
	// 引数：y	アニメーションの位置Y
	void hit(const float x, const float y);
};
#endif // !_ATTACK_EFFECT_H
