//==========================================================
/// @file
/// @brief    BraveAttackCollisionクラス
/// @author   阿部拳之
///
/// @attention  プレイヤー（勇者）による攻撃の当たり判定用のエンティティを出現させるクラスです。

//==========================================================
#ifndef _ATTACK_COLLISION_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _ATTACK_COLLISION_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "constants.h"
#include "attackEffect.h"

//==========================================================
// BraveAttackCollisionクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace braveAttackCollisionNS
{
	const int WIDTH = 24;				// 画像の幅（各フレーム）
	const int HEIGHT = 96;				// 画像の高さ
	const float COLLISION_TIME = 0.3f;	// 攻撃の当たり判定が出ている時間
}

//==========================================================
// プレイヤー（勇者）による攻撃の当たり判定用のエンティティを出現させるクラスです。
class BraveAttackCollision : public Entity
{
private:
	float mCollisionTimer;			// 攻撃の当たり判定が出ている時間を計測するタイマー
	AttackEffect mAttackEffect;		// 攻撃の衝撃派
public:
	// コンストラクタ
	BraveAttackCollision();

	//==========================================================
	// 継承されたメンバー関数
	//==========================================================

	// Update
	// 攻撃のアニメーションと自身のエンティティを更新する。
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
	void update(const float frameTime);
	// パラメータリセット
	// roundStart()内で呼び出される
	virtual void reset();

	//==========================================================
	// 新しいメンバー関数
	//==========================================================
	
	// Attack
	// 勇者から攻撃を発動
	// 指定した位置にこのエンティティをアクティブにして出現させる
	// 引数：centerX	攻撃を行ったエンティティの中心のX方向の位置
	// 引数：centerY	攻撃を行ったエンティティの中心のY方向の位置
	// 引数：width		攻撃を行ったエンティティの幅
	// 引数：height		攻撃を行ったエンティティの高さ
	// 引数：direction	攻撃を行ったエンティティが向いている方向
	void attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction);

	//==========================================================
	// getter
	//==========================================================

	// 攻撃の衝撃派の画像を戻す
	// 戻り値：攻撃用アニメーションの画像
	AttackEffect& getAttackEffect() { return mAttackEffect; }
};

#endif // !_ATTACK_COLLISION_H
