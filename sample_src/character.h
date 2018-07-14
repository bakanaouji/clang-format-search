//==========================================================
/// @file
/// @brief    Characterクラス
/// @author   阿部拳之
///
/// @attention  プレイヤーや敵クラスの基本となるクラス。
///				継承することで使用可能です。
///				継承なしではオブジェクトの生成はできません。

//==========================================================
#ifndef _CHARACTER_H	// このファイルが複数の箇所でインクルードされる場合に、
#define _CHARACTER_H	// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "map.h"

//==========================================================

// Characterクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace characterNS
{
	enum DIRECTION { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NONE = 4};				// キャラクターの向き（上下左右）
	enum STATE { MOVE, ATTACK, SECOND_ATTACK, GAURD , PRE_ATTACK , WAIT , DEATH };	// 状態（移動中、攻撃中、第二撃中、ガード中、攻撃直前、待機、死亡）
}

//==========================================================
// プレイヤーや敵クラスの基本となるクラスです。
// 継承することで使用可能です。
// 継承なしではオブジェクトの生成はできません。
class Character : public Entity
{
protected:
	characterNS::DIRECTION mDirection;	// キャラクターの向き（上下左右）
	int mOldStartFrame;					// 攻撃アニメーション終了後に戻るべきアニメーションフレーム保存用
	int mOldEndFrame;					// 攻撃アニメーション終了後に戻るべきアニメーションフレーム保存用
	bool mAttackCollisionFlag;			// 攻撃用の衝突判定を出現させるフラグ。攻撃を開始してから一定時間後にtrueとなる。時間を調整することによってアニメーションと同期させるのが目的。
	Map *mMap;							// ステージマップへのポインタ
	characterNS::STATE mState;			// 状態（移動中、攻撃中、第二撃中、ガード中、攻撃直前、待機、死亡）
	int mMoveUpStartFrame;				// 上方向移動のアニメーションの最初のフレーム
	int mMoveUpEndFrame;				// 上方向移動のアニメーションの最後のフレーム
	int mMoveRightStartFrame;			// 右方向移動のアニメーションの最初のフレーム
	int mMoveRightEndFrame;				// 右方向移動のアニメーションの最後のフレーム
	int mMoveDownStartFrame;			// 下方向移動のアニメーションの最初のフレーム
	int mMoveDownEndFrame;				// 下方向移動のアニメーションの最後のフレーム
	int mMoveLeftStartFrame;			// 左方向移動のアニメーションの最初のフレーム
	int mMoveLeftEndFrame;				// 左方向移動のアニメーションの最後のフレーム
	int mAttackUpStartFrame;			// 上方向攻撃のアニメーションの最初のフレーム
	int mAttackUpEndFrame;				// 上方向攻撃のアニメーションの最後のフレーム
	int mAttackRightStartFrame;			// 右方向攻撃のアニメーションの最初のフレーム
	int mAttackRightEndFrame;			// 右方向攻撃のアニメーションの最後のフレーム
	int mAttackDownStartFrame;			// 下方向攻撃のアニメーションの最初のフレーム
	int mAttackDownEndFrame;			// 下方向攻撃のアニメーションの最後のフレーム
	int mAttackLeftStartFrame;			// 左方向攻撃のアニメーションの最初のフレーム
	int mAttackLeftEndFrame;			// 左方向攻撃のアニメーションの最後のフレーム
public:
	// コンストラクタ
	Character();

	//==========================================================
	// 継承されたメンバー関数
	//==========================================================

	//==========================================================
	// 純粋仮想関数の宣言
	// これらの関数は、Gameを継承するクラス内で記述する必要があります。
	//==========================================================
	
	// Update
	// アニメーションや状態遷移、座標などの更新を行う。
	// 通常、フレームごとに1回呼び出す
	// frameTimeは、移動とアニメーションの速さを制御するために使用
	// 引数：frameTime　1フレームで経過した時間
	virtual void update(const float frameTime) = 0;
	// 人工知能。NPCの行動を決定するのに使用
	virtual void ai(const float frameTime, const Entity &ent) = 0;
	// キャラクターにダメージを与える関数
	virtual void damage(const WEAPON weapon) = 0;

	//==========================================================
	// 新しく追加するメンバー関数
	//==========================================================

	// パラメータリセット
	virtual void reset();
	// 移動可能かチェック
	// 引数：x	チェックしたい位置の座標X
	// 引数：y	チェックしたい位置の座標Y
	virtual bool checkCanMove(const float x, const float y);	
	// 死亡時に呼び出す関数
	void dead();
	// 移動時のアップデート関数
	// アニメーション以外をアップデート
	// 引数：frameTime　1フレームで経過した時間
	void updateMoving(const float frameTime);
	// 攻撃時のアップデート関数
	// 現状では通常のアップデート
	// 引数：frameTime　1フレームで経過した時間
	void updateAttacking(const float frameTime);
	// 移動中へと状態を遷移
	void changeStateToMove();

	//==========================================================
	// setter
	//==========================================================
	
	// マップへのポインタをセットする関数
	// 引数：map	マップオブジェクト
	void setMapPointer(Map& map) { mMap = &(map); }
	// 状態をセットする関数
	// 引数：state	状態（移動中、攻撃中、第二撃中、ガード中、攻撃直前、待機、死亡）
	void setState(const characterNS::STATE state) { mState = state; }
	// 向いている方向をセットする
	// 引数：dir	キャラクターの向き
	void setDirection(const characterNS::DIRECTION dir);

	//==========================================================
	// getter
	//==========================================================
	
	// 攻撃用の衝突判定を出現させるフラグを返す関数。攻撃状態に遷移した直後にtrueとなって、それ以外はfalse
	// 戻り値：攻撃用のエンティティを出現させるかどうか
	bool getAttackCollisionFlag() { return mAttackCollisionFlag; }
	// 向いている方向を返す関数
	// 戻り値：キャラクターの向いている方向
	characterNS::DIRECTION getDirection() { return mDirection; }
	// 状態を返す関数
	// 戻り値：状態（移動中、攻撃中、第二撃中、ガード中、攻撃直前、待機、死亡）
	characterNS::STATE getState() { return mState; }

};

#endif // !_CHARACTER_H
